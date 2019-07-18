#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicWarrior.h"
#include "EnemySpawner.generated.h"


UCLASS()
class TWALIENS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

private:
	FTimerHandle SpawnerTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
		int numberToSpawn = 3;

	UPROPERTY(EditAnywhere, Category = Default)
		class UBoxComponent* SpawnVolume;

	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
		bool isActivated = false;

	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
		bool isPassive = false;

	UPROPERTY(EditAnywhere, category = "Spawner Settings")
		//Time for the spawners to start working (from the start of the game)
		float TimeToStartSpawning = 1.0f;

	UPROPERTY(EditAnywhere, category = "Spawner Settings")
		float TimeBetweenSpawns = 10.0f;

	int activeEnmies;
public:
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, category = "Spawner Settings")
		class APoolManager* ObjectPool;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//TSubclassOf<ABasicWarrior> EnemyToSpawn;

	/*UPROPERTY(EditDefaultsOnly, Category = Default)
		UStaticMeshComponent* Meshcomp;*/


	UFUNCTION(BlueprintCallable)
		void SpawnEnemy();

	UFUNCTION(BlueprintCallable)
		void SpawnEnemyWithReference(TArray<class ABasicWarrior *> & Enemies);
	
	UFUNCTION(BlueprintCallable)
		bool GetActivated();

	//
	UFUNCTION(BlueprintCallable)
		void SetIsActivated(bool flag);

	UFUNCTION(BlueprintCallable)
		void SetIsPassive(bool flag);

	UFUNCTION(BlueprintCallable)
		bool GetIsPassive(bool flag);
};

