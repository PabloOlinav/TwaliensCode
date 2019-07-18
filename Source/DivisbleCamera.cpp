// Fill out your copyright notice in the Description page of Project Settings.

#include "DivisbleCamera.h"
#include "GameViewportDivisble.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "CameraLimitsComponent.h"

// Sets default values
ADivisbleCamera::ADivisbleCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//ViewportDivisible = Cast<UGameViewportDivisble>(GetWorld()->GetGameViewport());
	

	// Creating components
	CameraComponnt = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArmComponnt = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	StaticMeshComponnt = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	//Setting components position
	RootComponent = StaticMeshComponnt;
	SpringArmComponnt->SetupAttachment(RootComponent);
	CameraComponnt->SetupAttachment(SpringArmComponnt, USpringArmComponent::SocketName);

	// Giving a camera distance
	TrgtArmLength = MinDistanceTrgtArmLength;
	SpringArmComponnt->TargetArmLength = TrgtArmLength;

	SpringArmComponnt->bDoCollisionTest = false;

}

// Called when the game starts or when spawned
void ADivisbleCamera::BeginPlay()
{
	Super::BeginPlay();

	//If camera can be inited
	if (CanBeInited())
	{
		Init();
		HasBeenInited = true;
	}
	else
	{
		HasBeenInited = false;
	}
}

void ADivisbleCamera::Init()
{
	if (PlayerEnum != EPlayerNumberIndex::None)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPlayerNumberIndex"), true);

		if (EnumPtr)
			FollowedPlayerIndex = EnumPtr->GetIndexByValue((int64)PlayerEnum);
	}

	Player = UGameplayStatics::GetPlayerController(GetWorld(), FollowedPlayerIndex);
	Player->SetViewTarget(this);

	if (PlayerEnum == EPlayerNumberIndex::Player0)
	{
		PlayerTwo = UGameplayStatics::GetPlayerController(GetWorld(), 1);
		StaticMeshComponnt->SetRelativeLocation(GetAverageLocationBetweenPlayers());

		//Division
		ViewportDivisible = Cast<UGameViewportDivisble>(GetWorld()->GetGameViewport());
		StaticMeshComponnt->SetRelativeLocation(GetAverageLocationBetweenPlayers());
	}

	else if (PlayerEnum == EPlayerNumberIndex::Player1)
	{
		this->SetActorHiddenInGame(true);
		//this->SetActorTickEnabled(false);
	}

	RotationAngles.Yaw = Player->GetPawn()->GetActorRotation().Yaw;

	// Corresponding Angle for 3/4 camera vision
	SpringArmComponnt->SetWorldRotation(RotationAngles);
}

// Called every frame
void ADivisbleCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasBeenInited && CanBeInited())
	{
		Init();
		HasBeenInited = true;
	}

	if (Player->GetPawn())
	{

		if (PlayerEnum == EPlayerNumberIndex::Player0)
		{

			if (DistanceBetweenPlayers() < SeparationDistance)
			{
				ViewportDivisible->CancelSplitscreen();
				StaticMeshComponnt->SetRelativeLocation(GetAverageLocationBetweenPlayers());
			}

			else if (DistanceBetweenPlayers() > SeparationDistance)
			{
				ViewportDivisible->SetVerticalOrientation();
				ViewportDivisible->BeginSplitscreen();

				StaticMeshComponnt->SetRelativeLocation(Player->GetPawn()->GetActorLocation());
			}
		}

		if (PlayerEnum == EPlayerNumberIndex::Player1)
		{
			StaticMeshComponnt->SetRelativeLocation(Player->GetPawn()->GetActorLocation());

		}
	}
}

bool ADivisbleCamera::CanBeInited()
{
	return UGameplayStatics::GetPlayerController(GetWorld(), 0) && UGameplayStatics::GetPlayerController(GetWorld(), 1);
}

FVector ADivisbleCamera::GetAverageLocationBetweenPlayers()
{
	if (Player->GetPawn() && PlayerTwo->GetPawn())
	{
		FVector AverageVectorBetweenPlayers = Player->GetPawn()->GetActorLocation() + PlayerTwo->GetPawn()->GetActorLocation();
		AverageVectorBetweenPlayers = AverageVectorBetweenPlayers / 2;
		return	AverageVectorBetweenPlayers;
	}

	return FVector::ZeroVector;
}

FVector ADivisbleCamera::InterpolatingCameraCenterPoint(float DeltaTime)
{
	return FVector();
}

FVector ADivisbleCamera::InterpolationToOriginalPoint(float DeltaTime)
{
	return FVector();
}

float ADivisbleCamera::DistanceBetweenPlayers()
{
	if (Player->GetPawn() && PlayerTwo->GetPawn())
	{
		float distance = FVector::Distance(Player->GetPawn()->GetActorLocation(), PlayerTwo->GetPawn()->GetActorLocation());
		return distance;
	}

	return 0;
}


