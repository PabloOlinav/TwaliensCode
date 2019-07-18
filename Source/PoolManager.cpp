// All rights reserved.

#include "PoolManager.h"
#include "BasicWarrior.h"
#include "ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Interfaces/IPooledObject.h"


// Sets default values
APoolManager::APoolManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APoolManager::BeginPlay()
{
	Super::BeginPlay();

	//Initialize pointer to non valid position
	StackPointer = -1;

	//Reserve user amount size for objects
	ActorPool.Reserve(Size);

	//instantiate all pool objects
	FillPool();
}

// Called every frame
void APoolManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

IIPooledObject* APoolManager::GetActorFromPool()
{
	//If stack has any element
	if (StackPointer >= 0)
	{
		//Get last valid element
		IIPooledObject* ActorFromPool = ActorPool[StackPointer];

		//remove from array
		ActorPool.RemoveAt(StackPointer);

		//decrement stack head
		--StackPointer;

		//do activation stuff
		ActorFromPool->OnObjectExitsPool();

		//return object
		return ActorFromPool;
	}

	//In case we have no elements return null
	return nullptr;
}

void APoolManager::ReturnActorToPool(IIPooledObject* PooledActor)
{
	//We put element into array
	ActorPool.Add(PooledActor);
	//PooledActor->SetActorLocation(GetActorLocation());

	//do deactivation stuff
	PooledActor->OnObjectEnterPool();

	//increment stack head
	++StackPointer;
}

void APoolManager::SetPoolSize(int PoolSize)
{
	Size = PoolSize;
}

int APoolManager::GetPoolSize()
{
	return  StackPointer;
}

void APoolManager::FillPool()
{
	UWorld* world = GetWorld();

	//if Designer define class of pools object
	if (ObjectClassToSpawn)
	{
		//and world exists
		if (world)
		{
			//for size elements
			for (int i = 0; i < Size; ++i) 
			{
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;

				//Instantiate a new object with very basic settings
				IIPooledObject* PooledObject = world->SpawnActor<IIPooledObject>(ObjectClassToSpawn, GetActorLocation(), FRotator::ZeroRotator, spawnParams);

				//If instantiation is success
				if (PooledObject)
				{
					//add object to pool
					ActorPool.Add(PooledObject);

					//set owner pool of instantiated object this object
					PooledObject->SetOwnerPool(*this);

					//do deactivation stuff
					PooledObject->OnObjectEnterPool();

					//increment stack head
					++StackPointer;
				}
			}
		}
	}
}


