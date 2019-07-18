// All rights reserved.

#include "ProjectileA.h"
#include "Components/StaticMeshComponent.h"
#include "LifeFunctionality.h"
#include "PoolManager.h"
#include "TimerManager.h"
#include "Engine/StaticMesh.h"
#include "ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"
#include "ParticlesSceneComponent.h"
#include "Engine/Engine.h"

// Sets default values
AProjectileA::AProjectileA() : damage(20)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// We create the mesh sphere for the projectile
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	
	//Set the radious of the sphere to 15
	CollisionComponent->InitSphereRadius(15.0f);

	//Set Collision values
	CollisionComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Owner;
	CollisionComponent->SetCollisionProfileName(FName("BlockAllDynamic"));
	
	//Set the component as root, as we did with blueprint editor
	RootComponent = CollisionComponent;

	//Creating mesh for projectile
	MeshComp = CreateAbstractDefaultSubobject<UStaticMeshComponent>("MeshComponent");

	MeshComp->SetCollisionProfileName(FName("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshFinder(TEXT("/Game/Weapons/Ammunition/SM_Bullet_01"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> ProjectileMaterialFinder(TEXT("/Game/Weapons/Materials/M_Bullet_01"));

	if (ProjectileMeshFinder.Succeeded())
	{
		MeshComp->SetStaticMesh(ProjectileMeshFinder.Object);
	}
	if (ProjectileMaterialFinder.Succeeded())
	{
		MeshComp->SetMaterial(0, ProjectileMaterialFinder.Object);
	}

	MeshComp->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));

	MeshComp->SetupAttachment(CollisionComponent);

	//Setting the initial parameters for the projectile
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 4000.0f;
	ProjectileMovementComponent->MaxSpeed = 4000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = .0f;

	LifeTime = 7.0;
}

// Called when the game starts or when spawned
void AProjectileA::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectileA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileA::NotifyHit(class UPrimitiveComponent * MyComp, AActor * Other, class UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	bool IsAllowed = false;

	if (!Other && OtherComp)
	{
		Destroy();
		return;
	}

	for (int i = 0; i < Other->Tags.Num() && !IsAllowed; i++)
	{
		IsAllowed = AllowedTagsToDamage.Contains(Other->Tags[i]);
	}

	if (IsAllowed)
	{
		TSet<UActorComponent*> Components = Other->GetComponents();
		ILifeFunctionality * LifeComponent = nullptr;

		UParticlesSceneComponent * PS_Componnt = nullptr;
		TArray<USceneComponent*> ChildComponents;
		Other->GetRootComponent()->GetChildrenComponents(true, ChildComponents);

		for (UActorComponent * Component : Components)
		{
			LifeComponent = Cast<ILifeFunctionality>(Component);
			if(LifeComponent)
			{
				break;
			}
		}

		for (USceneComponent* comps : ChildComponents)
		{
			PS_Componnt = Cast<UParticlesSceneComponent>(comps);
			if (PS_Componnt)
			{
				break;
			}
		}

		if (LifeComponent && PS_Componnt)
		{
			LifeComponent->SetHitPoints(-damage);
			PS_Componnt->ActivateParticles();
		}
	}

	if(OwnerPool)
	{
		ReturnToPool();
		GetWorldTimerManager().ClearTimer(LifeTimeHanddle);
	}
	else
	{
		this->Destroy();
	}
}

// Function that initializes the projectile's velocity in the shoot direction.
void AProjectileA::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectileA::AddAllowedTagsToDamage(FName TagName)
{
	AllowedTagsToDamage.Add(TagName);
}

void AProjectileA::ResetAllowedTagsToDamage()
{
	AllowedTagsToDamage.Empty();
}


void AProjectileA::OnObjectExitsPool()
{
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMovementComponent->Activate();
	
	GetWorldTimerManager().SetTimer(LifeTimeHanddle, this, &AProjectileA::ReturnToPool, LifeTime, false);	
}


void AProjectileA::OnObjectEnterPool()
{
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovementComponent->Deactivate();
	SetActorLocation(OwnerPool->GetActorLocation());
}
