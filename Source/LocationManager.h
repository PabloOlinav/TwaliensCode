// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LocationManager.generated.h"

USTRUCT(BlueprintType)
struct FRegion
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector> Region;
};

UCLASS()
class TWALIENS_API ALocationManager : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TArray<FRegion> Regions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool CanNavigateBetweenRegions;

public:	
	// Sets default values for this actor's properties
	ALocationManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetRandomLocation(FVector & Destination, int RegionIndex) const;
};
