// All rights reserved.

#include "ShieldComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UShieldComponent::UShieldComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	ComponentTags.Add("Shield");
}


// Called when the game starts
void UShieldComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UShieldComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UShieldComponent::SetDamageReduction(float ReductionPercentage)
{
	DamageReduction = ReductionPercentage;
}

float UShieldComponent::GetDamageReduction()
{
	return DamageReduction;
}

