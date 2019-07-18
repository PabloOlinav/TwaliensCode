// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraLimitsComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UCameraLimitsComponent::UCameraLimitsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCameraLimitsComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
}


// Called every frame
void UCameraLimitsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	InterpolatingCameraPosition(this->GetOwner(), 0, DeltaTime);

}

void UCameraLimitsComponent::Limits(const FVector2D &ScreenPositionCoord)
{
	if (ScreenPositionCoord.X < LeftLimit ||
		ScreenPositionCoord.X > RightLimit ||
		ScreenPositionCoord.Y < TopLimit ||
		ScreenPositionCoord.Y > BottomLimit)
	{
		IsInTheLimit = true;

		if (ScreenPositionCoord.X < LeftLimit)
			IsInLeftLimit = true;
		else if (ScreenPositionCoord.X > RightLimit)
			IsInRightLimit = true;
		else if (ScreenPositionCoord.Y < TopLimit)
			IsInTopLimit = true;
		else if (ScreenPositionCoord.Y > BottomLimit)
			IsInBottomLimit = true;

	}

	else if (ScreenPositionCoord.X > LeftLimit &&
		ScreenPositionCoord.X < RightLimit ||
		ScreenPositionCoord.Y > TopLimit &&
		ScreenPositionCoord.Y < BottomLimit)
	{
		IsInTheLimit = false;
		IsInBottomLimit = false;
		IsInLeftLimit = false;
		IsInRightLimit = false;
		IsInTopLimit = false;
	}
}

void UCameraLimitsComponent::InterpolatingCameraPosition(const AActor* Player, const int& PlayerIndex, float DeltaTime)
{
	//ScreenPositionCoord;
	const APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
	PlayerController->ProjectWorldLocationToScreen(Player->GetActorLocation(), ScreenPositionCoord, false);

	Limits(ScreenPositionCoord);

}

