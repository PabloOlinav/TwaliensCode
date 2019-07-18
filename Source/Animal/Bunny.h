// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/EnvironmentalAnimal.h"
#include "Bunny.generated.h"

UCLASS()
class TWALIENS_API ABunny : public ACharacter, public IEnvironmentalAnimal
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Bunny")
	int RegionIndex;

	float WalkSpeed;

public:	
	// Sets default values for this character's properties
	ABunny();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE virtual int GetRegionIndex() const { return RegionIndex; }
	FORCEINLINE virtual void SetRegionIndex(int NewRegionIndex) { RegionIndex = NewRegionIndex; }

	UFUNCTION(BlueprintCallable)
	void StopMovement();

	UFUNCTION(BlueprintCallable)
	void StartMovement();
};
