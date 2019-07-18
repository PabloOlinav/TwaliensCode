// All rights reserved.

#include "DistanceWarrior.h"
#include "ConstructorHelpers.h"
#include "LifeComponent.h"
#include "LifeFunctionality.h"
#include "AIController.h"
#include "CoinLoot.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "ShieldComponent.h"
#include "PoolManager.h"

// Sets default values
ADistanceWarrior::ADistanceWarrior()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	////static ConstructorHelpers::FClassFinder<AActor> Coins(TEXT("/Game/Weapons/Ammunition/BP_Coins"));
	////static ConstructorHelpers::FObjectFinder<UStaticMesh> ShieldMesh(TEXT("/Game/Weapons/shield"));

	//Initializing components.	

	//Enemy Weapon

	Shield = CreateDefaultSubobject<UShieldComponent>(TEXT("Weapon"));
	Shield->SetupAttachment(GetMesh(), FName("LeftHandSocket"));

	//if(ShieldMesh.Succeeded())
	//{
	//	Shield->SetStaticMesh(ShieldMesh.Object);
	//}

	Shield->SetRelativeLocation(FVector(-32, -18, 1.5));
	Shield->SetRelativeRotation(FRotator(90, 185, 0));
	Shield->SetRelativeScale3D(FVector(15, 15, 15));

	ShootingGun = CreateDefaultSubobject<UWeapon>(TEXT("ShootingGunComponent"));
	ShootingGun->SetupAttachment(Shield);
	ShootingGun->SetProjectileTag("Player");

	ShootingGun->SetRelativeLocation(FVector(0, 1, 2.8));
	ShootingGun->SetRelativeRotation(FRotator(0, 90, 0));
	
	//HP Component
	HPComponent = CreateDefaultSubobject<ULifeComponent>("LifeComponent");
	AddOwnedComponent(HPComponent);

	CoinLootComponent = CreateDefaultSubobject<UCoinLoot>("CoinLootComponent");
	AddOwnedComponent(CoinLootComponent);

	//Initializing data members.
	IsAttacking = false;

	Tags.Add("Enemy");

	IsDead = true;
}

// Called when the game starts or when spawned
void ADistanceWarrior::BeginPlay()
{
	Super::BeginPlay();

	if (IsDead)
	{
		DisablePlayer();
	}
}

// Called every frame
void ADistanceWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Fire();
}

// Called to bind functionality to input
void ADistanceWarrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADistanceWarrior::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	HPComponent->UnregisterComponent();
	HPComponent->DestroyComponent();
}

void ADistanceWarrior::SetOwnerPool(APoolManager* Pool)
{
	OwnerPool = Pool;
}

bool ADistanceWarrior::GetIsAttacking()
{
	return IsAttacking;
}

void ADistanceWarrior::SetIsAttacking(bool NewAttackState)
{
	IsAttacking = NewAttackState;
}

void ADistanceWarrior::ActivateFiring()
{
	ShootingGun->Fire();
}

void ADistanceWarrior::OnDeath()
{
	//CoinLootComponent->SpawnCoins();
	////this->Controller->Destroy();
	//if (OwnerPool)
	//{
	//	OwnerPool->ReturnActorToPool(this);
	//}

	//this->Destroy();
}

void ADistanceWarrior::Fire()
{
	ShootingGun->Fire();
}

void ADistanceWarrior::DisablePlayer()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	Cast<AAIController>(this->GetController())->RunBehaviorTree(nullptr);
	SetActorTickEnabled(false);
}

void ADistanceWarrior::EnablePlayer()
{

}

void ADistanceWarrior::SetIsDead(bool b)
{
	IsDead = b;
}

