// All rights reserved.

#include "SpecificLocationSpawner.h"
#include "PoolManager.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IPooledObject.h"
#include "ScriptInterface.h"

// Sets default values
ASpecificLocationSpawner::ASpecificLocationSpawner() : Super(), CurrentLocation(0), HasUsedAllLocations(false), Locations()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASpecificLocationSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpecificLocationSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASpecificLocationSpawner::AllLocationsHasBeenUsed()
{
	return HasUsedAllLocations;
}

void ASpecificLocationSpawner::Spawn(TScriptInterface<IIPooledObject> & SpawnedObject)
{
	IIPooledObject * SpawnObject = ObjectPool->GetActorFromPool();
	SpawnedObject.SetInterface(SpawnObject);
	SpawnedObject.SetObject(nullptr);
	if (SpawnObject)
	{
		AActor * SpawnActor = Cast<AActor, IIPooledObject>(SpawnObject);
		SpawnedObject.SetObject(SpawnActor);
		check(SpawnActor);

		SpawnActor->SetActorLocation(Locations[CurrentLocation++]);
		if (CurrentLocation >= Locations.Num())
		{
			CurrentLocation = 0;
			HasUsedAllLocations = true;
		}
	}
}

