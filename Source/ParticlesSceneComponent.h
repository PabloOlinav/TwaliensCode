// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ParticlesSceneComponent.generated.h"

class UParticleSystemComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWALIENS_API UParticlesSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UParticlesSceneComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UParticleSystemComponent* PS_Component;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ActivateParticles();

		
};
