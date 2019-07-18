// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ShieldComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWALIENS_API UShieldComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShieldComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	//Shield mesh
	UPROPERTY(EditAnywhere, Category = "Shield Settings")
		float DamageReduction;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void SetDamageReduction(float ReductionPercentage);

	UFUNCTION(BlueprintCallable)
		float GetDamageReduction();
};
