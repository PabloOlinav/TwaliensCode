// All rights reserved.

#include "Sting.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "Components/PrimitiveComponent.h"
#include "Twalien.h"
#include "ActiveCrypteckBall.h"
#include "LifeComponent.h"
#include "SharedLifeComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Crypteck.h"
#include "Dustpan.h"
#include "CharacterManager.h"
#include "BasicWarrior.h"

// Sets default values
ASting::ASting() : Speed(2500), Rotation(450), DamageToPlayer(15), DamageToBall(10), Pickable(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("StingCollider"));
	Collider->SetBoxExtent(FVector(15.215841f, 50.286522f, 49.960678f));
	Collider->SetGenerateOverlapEvents(true);
	Collider->SetNotifyRigidBodyCollision(true);
	Collider->SetEnableGravity(false);
	Collider->SetSimulatePhysics(true);
	Collider->SetCollisionProfileName(FName("Custom..."));
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collider->SetCollisionObjectType(ECC_WorldDynamic);
	Collider->SetCollisionResponseToAllChannels(ECR_Block);
	Collider->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	Collider->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);
	Collider->OnComponentHit.AddDynamic(this, &ASting::OnStingHit);
	Collider->SetUseCCD(true);
	Collider->SetRelativeRotation(FRotator(270, 0, 0));
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StingMesh"));
	Mesh->SetCollisionProfileName(FName("NoCollision"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Engine/BasicShapes/Cone.Cone"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Engine/VREditor/UI/DiscMaterial.DiscMaterial"));
	if (MeshFinder.Succeeded())
	{
		Mesh->SetStaticMesh(MeshFinder.Object);
	}
	if (MaterialFinder.Succeeded())
	{
		Mesh->SetMaterial(0, MaterialFinder.Object);
	}
	Mesh->SetRelativeScale3D(FVector(.3f, 1, 1));
	Mesh->SetupAttachment(RootComponent);

	Tags.Add("Crypteck's Sting");

}

// Called when the game starts or when spawned
void ASting::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASting::OnStingHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	ATwalien * Player = Cast<ATwalien>(OtherActor);
	
	if (Player && !Pickable)
	{
		Player->GetLifeComponent()->SetHitPoints(-DamageToPlayer);
		Destroy();
	}
	else if (OtherActor && ActorIsCrypteckBall(OtherActor->Tags) && Hittable)
	{
		AActiveCrypteckBall * Ball = Cast<AActiveCrypteckBall>(OtherActor);
		if(Ball)
		{
			Ball->GetSphereLifeComponent()->SetHitPoints(-DamageToBall);
		}
		Destroy();
	}
	else if (OtherActor && OtherActor->Tags.Contains("Boss Crypteck") && Hittable)
	{
		ACrypteck * Boss = Cast<ACrypteck>(OtherActor);

		if (Boss)
		{
			Boss->GetCryptecksLife()->SetHitPoints(-DamageToBall);
		}

		Destroy();
	}
	else if (OtherActor && OtherActor->Tags.Contains("Environment"))
	{
		this->Pickable = true;
		Collider->SetPhysicsLinearVelocity(FVector::ZeroVector);
		Collider->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		Collider->SetEnableGravity(true);
	}
	else if (OtherActor && OtherActor->Tags.Contains("Enemy") && Hittable)
	{
		ABasicWarrior * Enemy = Cast<ABasicWarrior>(OtherActor);
		if (Enemy)
		{
			Enemy->GetLifeComponent()->SetHitPoints(-35);
		}
	}
	else if (OtherActor && OtherActor->GetName().Contains("ForceField"))
	{
		Destroy();
	}
}

bool ASting::ActorIsCrypteckBall(const TArray<FName> & ActorTags)
{
	bool found = false;
	for (const FName Name : ActorTags)
	{
		found = Name.ToString().Contains("Crypteck's ball");
		if (found)
			return found;
	}
	return found;
}

// Called every frame
void ASting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASting::Init(const FVector & NewDirection, bool CrypteckAmmunition)
{
	if (CrypteckAmmunition)
	{
		Pickable = false;
		Hittable = false;
	}
	else
	{
		Pickable = false;
		Hittable = true;
	}

	Collider->SetPhysicsLinearVelocity(NewDirection * Speed);
	if (CrypteckAmmunition)
	{
		Collider->SetPhysicsAngularVelocityInDegrees(FVector(0, 0, 1) * Rotation);
	}
}

void ASting::ConvertToAmmo()
{
	Collider->SetEnableGravity(false);
}

void ASting::OnBeingSucked(const UDustpan & Dustpan)
{
	if (!CanBeSucked())
		return;

	FVector Location = Dustpan.GetComponentLocation();
	FVector Direction;

	//We calculate vector from me to dustpan
	Direction = Location - GetActorLocation();
	Direction.Normalize();

	//If we are close
	if (FVector::Dist(GetActorLocation(), Location) <= Dustpan.GetSuckedDistance())
	{
		Dustpan.GetPlayerManager()->GiveSomeAlternativeAmmo(1);

		IsSucked = true;
	}
	else
	{
		//We apply force to me
		Collider->SetPhysicsLinearVelocity(Direction * Dustpan.GetSuckVelocity());
	}
}

