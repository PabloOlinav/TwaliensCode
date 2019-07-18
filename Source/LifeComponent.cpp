// All rights reserved.

#include "LifeComponent.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
ULifeComponent::ULifeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


float ULifeComponent::GetMaxHitPoints() const
{
	return MaxAmountHitPoints;
}

// Called when the game starts
void ULifeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ResetHitPoints();
}


// Called every frame
void ULifeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float ULifeComponent::GetHitPoints() const
{
	return HitPoints;
}

void ULifeComponent::ResetHitPoints()
{
	HitPoints = MaxAmountHitPoints;
	OnLifeChanged.Broadcast();
}

void ULifeComponent::SetHitPoints(float DeltaHitPoints)
{

	if (HitPoints + DeltaHitPoints < MaxAmountHitPoints)
		HitPoints += DeltaHitPoints;
	else
		HitPoints = MaxAmountHitPoints;

	OnLifeChanged.Broadcast();

	if (HitPoints <= 0)
	{
		OnActorDeath.Broadcast();
	}
}

void ULifeComponent::AddDelegateToHPUpdate(const FScriptDelegate & Delegate)
{
	OnLifeChanged.Add(Delegate);
}

void ULifeComponent::AddDelegateToDeath(const FScriptDelegate & Delegate)
{
	OnActorDeath.Add(Delegate);
}

void ULifeComponent::RemoveDelegateFromHPUpdate(const FScriptDelegate & Delegate)
{
	OnLifeChanged.Remove(Delegate);
}

void ULifeComponent::RemoveAllDelegatesFromHPUpdate()
{
	OnLifeChanged.Clear();
}

void ULifeComponent::RemoveDelegateFromDeath(const FScriptDelegate & Delegate)
{
	OnActorDeath.Remove(Delegate);
}

void ULifeComponent::RemoveAllDelegatesFromDeath()
{
	OnActorDeath.Clear();
}