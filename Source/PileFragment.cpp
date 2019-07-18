// All rights reserved.

#include "PileFragment.h"
#include "ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Materials/MaterialInterface.h"
#include "Dustpan.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SphereComponent.h"
#include "Twalien.h"

// Sets default values
APileFragment::APileFragment() : ISuckableObjects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Game/Environment/Altar/SM_AltarPiecePushB_01"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshMaterial(TEXT("/Game/Environment/Materials/MI_AltarPiece_01"));

	// PS Location
	ConstructorHelpers::FObjectFinder<UParticleSystem> EnergyEffect(TEXT("/Game/Effects/Particles/PS_AlPieceEnergy_01"));

	FragmentCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Fragment Collider"));
	FragmentCollider->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	FragmentCollider->SetCollisionProfileName(FName("CoinCollisionDefault"));
	FragmentCollider->SetSimulatePhysics(true);
	RootComponent = FragmentCollider;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetSphereRadius(250);
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionProfileName(FName("AimingAssist"));

	SphereComponentBeginOverlap.BindUFunction(this, FName("OnSphereComponentBeginOverlap"));
	SphereComponent->OnComponentBeginOverlap.Add(SphereComponentBeginOverlap);

	SphereComponentEndOverlap.BindUFunction(this, FName("OnSphereComponentEndOverlap"));
	SphereComponent->OnComponentEndOverlap.Add(SphereComponentEndOverlap);

	FragmentMesh = CreateDefaultSubobject<UStaticMeshComponent>("Fragment Mesh");

	if (MeshFinder.Succeeded())
	{
		FragmentMesh->SetStaticMesh(MeshFinder.Object);
	}

	FragmentMesh->SetRelativeLocation(FVector(0, 0, 0.0f));
	FragmentMesh->SetRelativeScale3D(FVector(.5f, .5f, .5f));

	if (MeshMaterial.Succeeded())
	{
		FragmentMesh->SetMaterial(0, MeshMaterial.Object);
	}

	FragmentMesh->SetupAttachment(RootComponent);
	FragmentMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FragmentMesh->bRenderCustomDepth = true;
	FragmentMesh->CustomDepthStencilValue = 5;

	Tags.Add("Pile's Fragment");

	//Particle System
	PS_PileFragment = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PS_PileFragmnt"));
	PS_PileFragment->SetupAttachment(RootComponent);
	PS_PileFragment->SetRelativeLocation(FVector::ZeroVector);
	PS_PileFragment->SetRelativeScale3D(FVector::OneVector);

	if (EnergyEffect.Succeeded())
	{
		PS_PileFragment->SetTemplate(EnergyEffect.Object);
	}
}

// Called when the game starts or when spawned
void APileFragment::BeginPlay()
{
	Super::BeginPlay();

	if(PS_PileFragment->GetComponentLocation() != FVector::ZeroVector)
		PS_PileFragment->SetRelativeLocation(FVector::ZeroVector);

	if(PS_PileFragment->GetComponentScale() != FVector::OneVector)
		PS_PileFragment->SetRelativeScale3D(FVector::OneVector);

	if (IsValid(FragmentMesh->GetMaterial(1)))
	{
		Fresnel = FragmentMesh->CreateDynamicMaterialInstance(1);
		Fresnel->SetScalarParameterValue(FName("Lifetime"), 0.0f);
	}

}

// Called every frame
void APileFragment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APileFragment::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Fresnel && Cast<ATwalien>(OtherActor))
	{
		Fresnel->SetScalarParameterValue(FName("Lifetime"), 1.0f);
	}
}

void APileFragment::OnSphereComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Fresnel && Cast<ATwalien>(OtherActor))
	{
		Fresnel->SetScalarParameterValue(FName("Lifetime"), 0.0f);
	}
}

void APileFragment::OnItemBecomeDisable()
{
	SetActorRotation(FRotator::ZeroRotator);
	FragmentCollider->SetComponentTickEnabled(false);
	//FragmentCollider->BodyInstance.bLockTranslation = true;
	//FragmentCollider->BodyInstance.bLockRotation = true;
	FragmentCollider->SetSimulatePhysics(false);
	FragmentCollider->SetPhysicsLinearVelocity(FVector::ZeroVector);
	IsSuckExclusive = false;
	FragmentCollider->SetMobility(EComponentMobility::Static);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(Fresnel)
	{
		Fresnel->SetScalarParameterValue(FName("Lifetime"), 0.0f);
	}
}


void APileFragment::OnItemBecomeEnable()
{
	//FragmentCollider->BodyInstance.bLockTranslation = false;
	//FragmentCollider->BodyInstance.bLockRotation = false;
	FragmentCollider->SetSimulatePhysics(true);
	FragmentCollider->SetMobility(EComponentMobility::Movable);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (Fresnel)
	{
		Fresnel->SetScalarParameterValue(FName("Lifetime"), 0.0f);
	}
}

void APileFragment::OnBeingSucked(const UDustpan & Dustpan)
{
	FVector Location = Dustpan.GetComponentLocation();
	FVector Direction;

	//first we check if collider is movable, if not suck does nothing
	if (FragmentCollider->Mobility.GetValue() != EComponentMobility::Movable)
	{
		return;
	}

	//We calculate vector from coin to dustpan
	Direction = Location - GetActorLocation();
	Direction.Normalize();

	//If Coin is close
	if (FVector::Dist(GetActorLocation(), Location) <= Dustpan.GetSuckedDistance())
	{
		//Removed previously Applied force
		FragmentCollider->SetPhysicsLinearVelocity(FVector::ZeroVector);

		//Stop the fragment in suck position
		SetActorLocation(Location);

		IsSuckExclusive = true;
	}
	else
	{
		//We apply force to coin
		FragmentCollider->SetPhysicsLinearVelocity(Direction * Dustpan.GetSuckVelocity());

		IsSuckExclusive = false;
	}
}

void APileFragment::OnObjectExitsPool()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	OnItemBecomeEnable();
	PS_PileFragment->Activate(true);
}

void APileFragment::OnObjectEnterPool()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetActorLocation(OwnerPool->GetActorLocation());
	OnItemBecomeDisable();
}
