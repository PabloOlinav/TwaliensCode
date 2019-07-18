// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "Twalien.h"
#include "CameraLimitsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WidgetComponent.h"
#include "Engine/Engine.h"

// Sets default values
ACamera::ACamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating components
	CameraComponnt = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArmComponnt = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	StaticMeshComponnt = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	//Setting components position
	RootComponent = StaticMeshComponnt;
//	SpringArmComponnt->AttachTo(RootComponent);
//	CameraComponnt->AttachTo(SpringArmComponnt, USpringArmComponent::SocketName);

	// Giving a camera distance
	TrgtArmLength = MinDistanceTrgtArmLength;
	SpringArmComponnt->TargetArmLength = TrgtArmLength;

	SpringArmComponnt->bDoCollisionTest = false;
}

// Called when the game starts or when spawned
void ACamera::BeginPlay()
{
	Super::BeginPlay();

	// Corresponding Angle for 3/4 camera vision
	SpringArmComponnt->SetWorldRotation(RotationAngles);

	// Adding players to Players array
	
	APlayerController* OurPlayerControllerOne = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APlayerController* OurPlayerControllerTwo = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	if (OurPlayerControllerOne != nullptr)
		Players.Add(OurPlayerControllerOne->GetPawn());

	if (OurPlayerControllerTwo != nullptr)
		Players.Add(OurPlayerControllerTwo->GetPawn());

	/*if (Players.Max() <= 0)
	{
		for (TActorIterator<ATwalien> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			APawn * CurrentMthfckr = *ActorItr;
			Players.Add(CurrentMthfckr);
		}
	}*/

	// Assigning CameraComponnt as Main Camera
	for (int i = 0; i < Players.Num(); ++i)
	{
		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(Players[i], i);
		OurPlayerController->SetViewTarget(this);
	}

	CamLimComponntPlayerOne = Players[0]->FindComponentByClass<UCameraLimitsComponent>();
	CamLimComponntPlayerTwo = Players[1]->FindComponentByClass<UCameraLimitsComponent>();

	StaticMeshComponnt->SetRelativeLocation(GetAverageLocationBetweenPlayers());

	


}

// Called every frame
void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActivateFollowAverage)
	{
		// Getting distances between each players and this object root
		distancePlayerOne = FVector::Distance(Players[0]->GetActorLocation(), StaticMeshComponnt->GetComponentLocation());
		distancePlayerTwo = FVector::Distance(Players[1]->GetActorLocation(), StaticMeshComponnt->GetComponentLocation());
		distanceBetweenPlayers = FVector::Distance(Players[0]->GetActorLocation(), Players[1]->GetActorLocation());

		ExpansionCondition(CamLimComponntPlayerOne, CamLimComponntPlayerTwo, distancePlayerOne, distancePlayerTwo);
				
		// Making the root stays on Players average location if any of them are reaching the limits
		if(!CamLimComponntPlayerOne->IsInTheLimit && !CamLimComponntPlayerTwo->IsInTheLimit && !IsExpanded)
		{
			StaticMeshComponnt->SetRelativeLocation(InterpolationToOriginalPoint(DeltaTime));
		}

		// In case someone is touching the limits make an interpolation in the center point (average), to create a smooth movement towards the new target
		else if (!CamLimComponntPlayerOne->IsInTheLimit && !CamLimComponntPlayerTwo->IsInTheLimit && IsExpanded ||
				  CamLimComponntPlayerOne->IsInTheLimit && !IsExpanded ||
				  CamLimComponntPlayerTwo->IsInTheLimit && !IsExpanded)
			{
				StaticMeshComponnt->SetRelativeLocation(InterpolatingCameraCenterPoint(DeltaTime));
			}

		// Expand
		if (IsExpanded)
		{
			TrgtArmLength = FMath::FInterpTo(TrgtArmLength, MaxDistanceTrgtArmLength, DeltaTime, 5);
			SpringArmComponnt->TargetArmLength = TrgtArmLength;
		
		}
		//Shrink
		else if (!IsExpanded)
		{
			TrgtArmLength = FMath::FInterpTo(TrgtArmLength, MinDistanceTrgtArmLength, DeltaTime, 5);
			SpringArmComponnt->TargetArmLength = TrgtArmLength;
			
		}

		// Alerting players about camera's expansion
		if(CamLimComponntPlayerOne->IsCameraExpanded != IsExpanded || CamLimComponntPlayerTwo->IsCameraExpanded != IsExpanded)
		{
			CamLimComponntPlayerOne->IsCameraExpanded = IsExpanded;
			CamLimComponntPlayerTwo->IsCameraExpanded = IsExpanded;
		}
		
	}
	else
		return;
}

FVector ACamera::GetAverageLocationBetweenPlayers()
{
	FVector AverageVectorBetweenPlayers = Players[0]->GetActorLocation() + Players[1]->GetActorLocation();
	AverageVectorBetweenPlayers = AverageVectorBetweenPlayers / 2;
	return	AverageVectorBetweenPlayers;
}

FVector ACamera::InterpolatingCameraCenterPoint(float DeltaTime)
{
	FVector passedLocation = StaticMeshComponnt->GetComponentLocation();
	FVector interpol = FMath::Lerp(passedLocation, GetAverageLocationBetweenPlayers(), DeltaTime * 5.f);
	return interpol;
}

FVector ACamera::InterpolationToOriginalPoint(float DeltaTime)
{
	FVector passedLocation = StaticMeshComponnt->GetComponentLocation();
	FVector interpol = FMath::Lerp(GetAverageLocationBetweenPlayers(),passedLocation, DeltaTime * 20.f);
	return interpol;
}

void ACamera::ExpansionCondition(const UCameraLimitsComponent* LimitCheckerPlayerOne, const UCameraLimitsComponent* LimitCheckerPlayerTwo, const float& distancePlayerOne, const float& distancePlayerTwo)
{
	if(!IsExpanded)
	{
		if (!LimitCheckerPlayerOne->IsInTheLimit && !LimitCheckerPlayerTwo->IsInTheLimit ||
			LimitCheckerPlayerOne->IsInTheLimit && !LimitCheckerPlayerTwo->IsInTheLimit ||
			!LimitCheckerPlayerOne->IsInTheLimit && LimitCheckerPlayerTwo->IsInTheLimit ||
			LimitCheckerPlayerOne->IsInTheLimit && LimitCheckerPlayerTwo->IsInTheLimit)
		{
			if (distancePlayerOne > 460.f && distancePlayerTwo > 460.f)
			{
				IsExpanded = true;
			}
		}
	}

	else if(IsExpanded)
	{
		if (!LimitCheckerPlayerOne->IsInTheLimit && !LimitCheckerPlayerTwo->IsInTheLimit ||
			LimitCheckerPlayerOne->IsInTheLimit && !LimitCheckerPlayerTwo->IsInTheLimit ||
			!LimitCheckerPlayerOne->IsInTheLimit && LimitCheckerPlayerTwo->IsInTheLimit )
		{
			if (distancePlayerOne < 400.f && distancePlayerTwo < 400.f ||
				distanceBetweenPlayers < 400.f && distancePlayerOne > 400.f && distancePlayerTwo > 400.f)
			{
				IsExpanded = false;
			}
		}
	}
}
