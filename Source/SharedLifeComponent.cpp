// All rights reserved.

#include "SharedLifeComponent.h"
#include "LifeComponent.h"

// Sets default values for this component's properties
USharedLifeComponent::USharedLifeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	SourceUpdateDelegate.BindUFunction(this, FName("OnSourceLifeChange"));
	SourceDeathDelegate.BindUFunction(this, FName("OnSourceLifeIsZero"));
}

// Called when the game starts
void USharedLifeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USharedLifeComponent::OnSourceLifeChange()
{
	OnLifeChanged.Broadcast();
}

void USharedLifeComponent::OnSourceLifeIsZero()
{
	OnActorDeath.Broadcast();
}

// Called every frame
void USharedLifeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//Return Actor Hit Points.
float USharedLifeComponent::GetHitPoints() const
{
	if (SourceLife)
	{
		return SourceLife->GetHitPoints();
	}
	else
		return -99999;
}

//Reset Actor Hit Points.
void USharedLifeComponent::ResetHitPoints()
{
	if (SourceLife)
		SourceLife->ResetHitPoints();
}

//Modify Actor Hit Points. 
//DeltaHitPoints -> Hit Points added to Actor Hit Points.
void USharedLifeComponent::SetHitPoints(float DeltaHitPoints)
{
	if(SourceLife)
		SourceLife->SetHitPoints(DeltaHitPoints);
}

//Add a function to life changes delegate.
void USharedLifeComponent::AddDelegateToHPUpdate(const FScriptDelegate & Delegate)
{
	OnLifeChanged.Add(Delegate);
}

//Add a function to delegate that broadcast whenever actor dies.
void USharedLifeComponent::AddDelegateToDeath(const FScriptDelegate & Delegate)
{
	OnActorDeath.Add(Delegate);
}

//Remove a function from life changes delegate.
void USharedLifeComponent::RemoveDelegateFromHPUpdate(const FScriptDelegate & Delegate)
{
	OnLifeChanged.Remove(Delegate);
}

//Clear all functions from life changes delegate.
void USharedLifeComponent::RemoveAllDelegatesFromHPUpdate()
{
	OnLifeChanged.Clear();
}

//Remove a function from death delegate.
void USharedLifeComponent::RemoveDelegateFromDeath(const FScriptDelegate & Delegate)
{
	OnActorDeath.Remove(Delegate);
}

//Clear all functions from death delegate.
void USharedLifeComponent::RemoveAllDelegatesFromDeath()
{
	OnActorDeath.Clear();
}

void USharedLifeComponent::SetCommonLifeComponent(ULifeComponent * Source)
{
	//If we had a source
	if (SourceLife)
	{
		//Remove all delegates for their
		SourceLife->RemoveDelegateFromHPUpdate(SourceUpdateDelegate);
		SourceLife->RemoveDelegateFromDeath(SourceDeathDelegate);
	}
	//Set new source
	SourceLife = Source;
	//Setup connections
	SourceLife->AddDelegateToHPUpdate(SourceUpdateDelegate);
	SourceLife->AddDelegateToDeath(SourceDeathDelegate);
}

ULifeComponent * USharedLifeComponent::GetCommonLifeComponent()
{
	return SourceLife;
}

float USharedLifeComponent::GetMaxHitPoints() const
{
	return SourceLife->GetMaxHitPoints();
}
