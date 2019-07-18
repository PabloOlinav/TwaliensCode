// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraLimitsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWALIENS_API UCameraLimitsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraLimitsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Settings")
	bool IsInTheLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Settings")
	bool IsCameraExpanded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Limits Values")
		float LeftLimit = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Limits Values")
		float RightLimit = 1300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Limits Values")
		float TopLimit = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Limits Values")
		float BottomLimit = 600.f;

	FVector2D ScreenPositionCoord;

	bool IsInLeftLimit;
	bool IsInRightLimit;
	bool IsInTopLimit;
	bool IsInBottomLimit;

	void InterpolatingCameraPosition(const AActor* Player, const int& PlayerIndex, float DeltaTime);

	void Limits(const FVector2D &ScreenPositionCoord);

		
};
