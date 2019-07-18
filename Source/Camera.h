// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera.generated.h"



class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;
class UCameraLimitsComponent;

UCLASS()
class TWALIENS_API ACamera : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//void ExpansionCondition( const float& distance);
	void ExpansionCondition(const UCameraLimitsComponent* LimitCheckerPlayerOne, const UCameraLimitsComponent* LimitCheckerPlayerTwo, const float& distancePlayerOne, const float& distancePlayerTwo);
	FVector GetAverageLocationBetweenPlayers();
	FVector InterpolatingCameraCenterPoint(float DeltaTime);
	FVector InterpolationToOriginalPoint (float DeltaTime);

private:
	float distancePlayerOne;
	float distancePlayerTwo;
	float distanceBetweenPlayers;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Players array
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "Settings")
		TArray<APawn*> Players;

	// Components
		UCameraComponent* CameraComponnt;
		USpringArmComponent* SpringArmComponnt;
		UStaticMeshComponent* StaticMeshComponnt;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Limits Settings")
		UCameraLimitsComponent* CamLimComponntPlayerOne;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Limits Settings")
		UCameraLimitsComponent* CamLimComponntPlayerTwo;


	UPROPERTY(EditAnywhere, category = "Settings")
		float TrgtArmLength;

	UPROPERTY(EditAnywhere, category = "Settings")
		FRotator RotationAngles;

	UPROPERTY(EditAnywhere, category = "Settings")
		float MinDistanceTrgtArmLength = 700.f;

	UPROPERTY(EditAnywhere, category = "Settings")
		float MaxDistanceTrgtArmLength = 1800.f;

	UPROPERTY(EditAnywhere, category = "Settings")
		bool ActivateFollowAverage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Settings")
		bool IsExpanded;
	UPROPERTY(EditAnywhere, category = "Settings")
		bool IsInTheLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Settings")
		bool ShowLimitsDebugLines;

};
