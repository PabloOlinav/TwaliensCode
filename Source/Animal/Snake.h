// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/EnvironmentalAnimal.h"
#include "Snake.generated.h"

UCLASS()
class TWALIENS_API ASnake : public ACharacter, public IEnvironmentalAnimal
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spider")
		int RegionIndex;

public:	
	// Sets default values for this character's properties
	ASnake();

	virtual int GetRegionIndex() const override;

	virtual void SetRegionIndex(int NewRegionIndex) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
