// All rights reserved.

#include "CoinLoot.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Coin.h"
#include "Components/PrimitiveComponent.h"
#include "LifeComponent.h"
#include "Ammo.h"

// Sets default values for this component's properties
UCoinLoot::UCoinLoot()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	LootClass = AAmmo::StaticClass();
}


// Called when the game starts
void UCoinLoot::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UCoinLoot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCoinLoot::SpawnCoins() const
{
	FVector ActorLocation = GetOwner()->GetActorLocation();
	FVector SpawnLocation = ActorLocation + SpawnOffset;
	FActorSpawnParameters spawnParams;
	FRotator rotation = FRotator{ 0, 0, 0 };
	
	spawnParams.Owner = GetOwner();
	spawnParams.Instigator = nullptr;

	for (int i = 0; i < AmountCoins; ++i)
	{
		float Sin, Cos;
		FVector ForceVector;
		UPrimitiveComponent * CoinPrimitive;

		AAmmo * Loot = GetOwner()->GetWorld()->SpawnActor<AAmmo>(LootClass, SpawnLocation, rotation, spawnParams);

		Sin = FMath::Sin(FMath::DegreesToRadians((360 / AmountCoins) * i));
		Cos = FMath::Cos(FMath::DegreesToRadians((360 / AmountCoins) * i));
		
		ForceVector.X = Cos * Radius;
		ForceVector.Y = Sin * Radius;
		ForceVector.Z = RelativeHeightForceVector * ForceToApply;

		CoinPrimitive = Loot->GetCollider();
		CoinPrimitive->AddForce(ForceVector, NAME_None, true);
	}
}