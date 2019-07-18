// All rights reserved.

#include "EnemySpawner.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "BasicWarriorController.h"
#include "Twalien.h"
#include "EnemySpawner.h"
#include "PoolManager.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ConstructorHelpers::FClassFinder<ABasicWarrior> BasicWarrior(TEXT("/Game/Characters/BasicWarrior/BP_BasicWarrior_0"));

	//EnemyToSpawn = BasicWarrior.Class;

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	//Meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SpawnVolume->SetupAttachment(RootComponent);
	


}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	//EnemyPool->FillPool(Cast<AActor>(EnemyToSpawn));
	UWorld* world = GetWorld();

	if (!isPassive && isActivated) {
		GetWorldTimerManager().SetTimer(SpawnerTimerHandle, this, &AEnemySpawner::SpawnEnemy, TimeBetweenSpawns, true, TimeToStartSpawning);
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AEnemySpawner::SpawnEnemy()
{
	UWorld* world = GetWorld();

	//Coords of the entire box component as spawnzone
	float xmin = SpawnVolume->GetComponentLocation().X - SpawnVolume->GetScaledBoxExtent().X;
	float xmax = SpawnVolume->GetComponentLocation().X + SpawnVolume->GetScaledBoxExtent().X;
	float ymin = SpawnVolume->GetComponentLocation().Y - SpawnVolume->GetScaledBoxExtent().Y;
	float ymax = SpawnVolume->GetComponentLocation().Y + SpawnVolume->GetScaledBoxExtent().Y;
	float z = SpawnVolume->GetComponentLocation().Z - SpawnVolume->GetScaledBoxExtent().Z;

	if (ObjectPool)
	{
		if (world)
		{
			for (int i = 1; i <= numberToSpawn && ObjectPool->GetPoolSize() >= 0; ++i)
			{
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;
				//FRotator Rotator = this->GetActorRotation();

				FVector Location = FVector(FMath::RandRange(xmin, xmax), FMath::RandRange(ymin, ymax), z);

				AActor* Object = Cast<AActor>(ObjectPool->GetActorFromPool());

				if (Object)
				{				
					//Enemy->SetActorLocationAndRotation(Location, Rotator);
					Object->SetActorLocation(Location);
				}
			}
		}
	}
}


void AEnemySpawner::SpawnEnemyWithReference(TArray<class ABasicWarrior*> & Enemies)
{
	UWorld* world = GetWorld();

	//Coords of the entire box component as spawnzone
	float xmin = SpawnVolume->GetComponentLocation().X - SpawnVolume->GetScaledBoxExtent().X;
	float xmax = SpawnVolume->GetComponentLocation().X + SpawnVolume->GetScaledBoxExtent().X;
	float ymin = SpawnVolume->GetComponentLocation().Y - SpawnVolume->GetScaledBoxExtent().Y;
	float ymax = SpawnVolume->GetComponentLocation().Y + SpawnVolume->GetScaledBoxExtent().Y;
	float z = SpawnVolume->GetComponentLocation().Z - SpawnVolume->GetScaledBoxExtent().Z;

	if (ObjectPool)
	{
		if (world)
		{
			for (int i = 1; i <= numberToSpawn && ObjectPool->GetPoolSize() >= 0; ++i)
			{
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;
				FRotator Rotator = this->GetActorRotation();

				FVector Location = FVector(FMath::RandRange(xmin, xmax), FMath::RandRange(ymin, ymax), z);

				ABasicWarrior* Enemy = Cast<ABasicWarrior>(ObjectPool->GetActorFromPool());

				if (Enemy)
				{
					Enemy->SetActorLocationAndRotation(Location, Rotator);
					Enemies.Add(Enemy);
				}
			}
		}
	}
}

bool AEnemySpawner::GetActivated()
{
	return isActivated;
}

void AEnemySpawner::SetIsActivated(bool flag)
{
	if (flag && isPassive) 
	{
		isActivated = flag;
		//th.BindUFunction(this, FName("SpawnEnemy"));
		GetWorldTimerManager().SetTimer(SpawnerTimerHandle, this, &AEnemySpawner::SpawnEnemy, TimeBetweenSpawns, true, TimeToStartSpawning);

	}
	else if (!flag && isPassive) 
	{
		GetWorldTimerManager().ClearTimer(SpawnerTimerHandle);
		Destroy();
	}
	
}

void AEnemySpawner::SetIsPassive(bool flag)
{
	isPassive = flag;
}

bool AEnemySpawner::GetIsPassive(bool flag)
{
	return isPassive;
}
