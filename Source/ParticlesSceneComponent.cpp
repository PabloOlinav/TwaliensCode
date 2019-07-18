// All rights reserved.

#include "ParticlesSceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UParticlesSceneComponent::UParticlesSceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	PS_Component = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponnt"));
	PS_Component->SetupAttachment(this);
}


// Called when the game starts
void UParticlesSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UParticlesSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UParticlesSceneComponent::ActivateParticles()
{
	PS_Component->ActivateSystem(true);
}

