// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerNumberIndex.h"
#include "DivisbleCamera.generated.h"

class UGameViewportDivisble;
class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;


//UENUM()
//enum class EPlayerNumberIndex :uint8
//{
//	Player0,
//	Player1,
//	None,
//	
//};

UCLASS()
class TWALIENS_API ADivisbleCamera : public AActor
{
	GENERATED_BODY()
	
private:
	UGameViewportDivisble* ViewportDivisible = nullptr;
	int32 FollowedPlayerIndex;
	APlayerController* Player;
	APlayerController* PlayerTwo = nullptr;

public:	
	// Sets default values for this actor's properties
	ADivisbleCamera();

	// Components
	UCameraComponent* CameraComponnt;
	USpringArmComponent* SpringArmComponnt;
	UStaticMeshComponent* StaticMeshComponnt;

	UPROPERTY(EditAnywhere, Category = "Enum")
		EPlayerNumberIndex PlayerEnum;

	//Properties
	UPROPERTY(EditAnywhere, category = "Settings")
		float TrgtArmLength;

	UPROPERTY(EditAnywhere, category = "Settings")
		FRotator RotationAngles;

	UPROPERTY(EditAnywhere, category = "Settings")
		float MinDistanceTrgtArmLength = 700.f;

	UPROPERTY(EditAnywhere, category = "Settings")
		float MaxDistanceTrgtArmLength = 1800.f;

	UPROPERTY(EditAnywhere, category = "Settings")
		float SeparationDistance = 400.f;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Init();
	bool HasBeenInited;
	bool CanBeInited(); 
	FVector GetAverageLocationBetweenPlayers();
	FVector InterpolatingCameraCenterPoint(float DeltaTime);
	FVector InterpolationToOriginalPoint(float DeltaTime);
	float DistanceBetweenPlayers();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};




