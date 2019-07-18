// All rights reserved.

#include "Weapon.h"
#include "ProjectileA.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "ConstructorHelpers.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Crypteck/Sting.h"
#include "PoolManager.h"

// Sets default values for this component's properties
UWeapon::UWeapon() : Ammunition(20), AltAmmunition(0), BaseFireRate(0.3f), SkillFireRate(0.15f), ProjectileTag("Enemy"), SkillDuration(30), Super()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	FireAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Gun Audio"));
	FireAudioComponent->SetupAttachment(this);
}


// Called when the game starts
void UWeapon::BeginPlay()
{
	Super::BeginPlay();

	// ...
	FireRate = BaseFireRate;
}


// Called every frame
void UWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	FindPool();
}

void UWeapon::Fire()
{
	// Attempt to fire a projectile.
	if (BulletPool && Ammunition > 0)
	{
		UWorld * World = GetWorld();

		if (World && !World->GetTimerManager().IsTimerActive(FireRateHandler))
		{	

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = Cast<APawn>(GetOwner());

			// Spawn the projectile at the muzzle.
			AProjectileA* Projectile = Cast<AProjectileA>(BulletPool->GetActorFromPool());
			
			//World->SpawnActor<AProjectileA>(ProjectileClass, GetComponentLocation(), GetComponentRotation(), SpawnParams);
			
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
				Projectile->ResetAllowedTagsToDamage();
				Projectile->AddAllowedTagsToDamage(ProjectileTag);
				//Projectile->SetOwnerPool(BulletPool);
				Projectile->SetActorLocation(GetComponentLocation());
				Projectile->SetActorRotation(GetComponentRotation());
				FVector LaunchDirection = GetForwardVector();
				Projectile->FireInDirection(LaunchDirection);

				if(FireAudioComponent->Event.Get())
				{
					FireAudioComponent->Play();
				}

				//Projectile->Enable();
				--Ammunition;


			}
			World->GetTimerManager().SetTimer(FireRateHandler, FireRate, false);
		}
	}
}


void UWeapon::AltFire()
{
	// Attempt to fire a projectile.
	if (AltAmmunition > 0)
	{
		UWorld * World = GetWorld();

		if (World && !World->GetTimerManager().IsTimerActive(FireRateHandler))
		{

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = Cast<APawn>(GetOwner());

			// Spawn the projectile at the muzzle.
			ASting * Projectile = World->SpawnActor<ASting>(GetComponentLocation(), GetComponentRotation(), SpawnParams);
			Projectile->Init(GetForwardVector(), false);
			--AltAmmunition;

			World->GetTimerManager().SetTimer(FireRateHandler, FireRate, false);
		}
	}
}

void UWeapon::WeaponSkill() 
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(SkillHandler)) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Skill Activated"));
		FireRate = SkillFireRate;	
		GetWorld()->GetTimerManager().SetTimer(SkillHandler, this, &UWeapon::DeactivateSkill, 1.f, false, SkillDuration);
	}
}

void UWeapon::DeactivateSkill() 
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Skill Desactivated"));
	FireRate = BaseFireRate;
}

void UWeapon::FindPool()
{
	class UWorld* const world = GetWorld();

	if (world)
	{
		for (TActorIterator<APoolManager> Itr(world); Itr; ++Itr)
		{
			APoolManager* FoundPool = *Itr;

			if (FoundPool && FoundPool->Tags.Contains("Bullet"))
			{
				BulletPool = FoundPool;
			}
		}
	}
}

int UWeapon::GetAmmunition() const
{
	return Ammunition; 
}

void UWeapon::TransferState(const UWeapon & Other)
{
	//Safety checks
	check(BulletPool == Other.BulletPool);
	check(ProjectileClass == Other.ProjectileClass);
	check(BaseFireRate == Other.BaseFireRate);
	check(SkillFireRate == Other.SkillFireRate);
	check(ProjectileTag == Other.ProjectileTag);

	//copy variables
	Ammunition = Other.Ammunition;
	AltAmmunition = Other.AltAmmunition;
	FireRate = Other.FireRate;

	//copy timers
	//FireRate
	if (GetWorld()->GetTimerManager().IsTimerActive(Other.FireRateHandler))
	{
		float FireRateRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(Other.FireRateHandler);
		GetWorld()->GetTimerManager().SetTimer(FireRateHandler, FireRateRemaining, false);
	}
	//Skill
	if (GetWorld()->GetTimerManager().IsTimerActive(Other.SkillHandler))
	{
		float SkillRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(Other.SkillHandler);
		GetWorld()->GetTimerManager().SetTimer(SkillHandler, this, &UWeapon::DeactivateSkill, SkillRemaining);
	}
}
