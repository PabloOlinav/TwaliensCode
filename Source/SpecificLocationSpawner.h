// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpecificLocationSpawner.generated.h"

UCLASS()
class TWALIENS_API ASpecificLocationSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpecificLocationSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool AllLocationsHasBeenUsed();

	UFUNCTION(BlueprintCallable)
	void Spawn(class TScriptInterface<class IIPooledObject> & SpawnedObject);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	int CurrentLocation;
	
	bool HasUsedAllLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings", meta = (AllowPrivateAccess = "true"))
	TArray<FVector> Locations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings", meta = (AllowPrivateAccess = "true"))
	class APoolManager * ObjectPool;

};
