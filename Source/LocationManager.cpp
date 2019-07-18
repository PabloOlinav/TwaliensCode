// All rights reserved.

#include "LocationManager.h"

// Sets default values
ALocationManager::ALocationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ALocationManager::BeginPlay()
{
	Super::BeginPlay();

	//Maps sould have at least one region
	//No Regions, no life
	check(Regions.Num() > 0);
}

// Called every frame
void ALocationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALocationManager::GetRandomLocation(FVector & Destination, int RegionIndex) const
{
	//If this fails, try to set a correct region index, you are doing a segmentation fault
	check(RegionIndex < Regions.Num());
	
	if (RegionIndex < Regions.Num())
	{
		if (CanNavigateBetweenRegions)
		{
			float RandRegion = FMath::RandRange(0, Regions.Num());
			float RandPos = FMath::RandRange(0, Regions[RandRegion].Region.Num() - 1);

			Destination = Regions[RandRegion].Region[RandPos];
		}
		else
		{
			float Rand = FMath::RandRange(0, Regions[RegionIndex].Region.Num() - 1);

			Destination = Regions[RegionIndex].Region[Rand];
		}
	}
	else
	{
		Destination = FVector::ZeroVector;
	}
}

