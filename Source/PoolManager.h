// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolManager.generated.h"

UCLASS()
class TWALIENS_API APoolManager : public AActor
{
	GENERATED_BODY()

private:

	int StackPointer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<class IIPooledObject*> ActorPool;

	UPROPERTY(EditAnywhere, Category = "Settings")
	int Size;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (MustImplement = "IPooledObject"))
	TSubclassOf<UObject> ObjectClassToSpawn;

public:	
	// Sets default values for this actor's properties
	APoolManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	IIPooledObject* GetActorFromPool();

	void ReturnActorToPool(IIPooledObject* PooledActor);

	void SetPoolSize(int PoolSize);

	int GetPoolSize();

	void FillPool();
};
