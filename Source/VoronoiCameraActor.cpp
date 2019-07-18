// All rights reserved.

#include "VoronoiCameraActor.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "PlayerNumberIndex.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PostProcessComponent.h"

// Sets default values
AVoronoiCameraActor::AVoronoiCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Creating components
	CameraComponnt = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArmComponnt = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	StaticMeshComponnt = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PostProcessingComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessingComponnt"));
	

	//Setting components position
	RootComponent = StaticMeshComponnt;
	SpringArmComponnt->SetupAttachment(RootComponent);
	CameraComponnt->SetupAttachment(SpringArmComponnt, USpringArmComponent::SocketName);
	PostProcessingComponent->SetupAttachment(RootComponent);

	// Giving a camera distance
	SpringArmComponnt->TargetArmLength = TrgtArmLength;

	SpringArmComponnt->bDoCollisionTest = false;
	
}

// Called when the game starts or when spawned
void AVoronoiCameraActor::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerEnum != EPlayerNumberIndex::None)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPlayerNumberIndex"), true);
		if (EnumPtr)
			FollowedPlayerIndex = EnumPtr->GetIndexByValue((int64)PlayerEnum);
	}

	Player = UGameplayStatics::GetPlayerController(GetWorld(), FollowedPlayerIndex);
	//Player->SetViewTarget(this);

	//RotationAngles.Yaw = Player->GetPawn()->GetActorRotation().Yaw;

	// Corresponding Angle for 3/4 camera vision
	SpringArmComponnt->SetWorldRotation(RotationAngles);

	SpringArmComponnt->TargetArmLength = TrgtArmLength;
	
}

// Called every frame
void AVoronoiCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (SpringArmComponnt->TargetArmLength == TrgtArmLength)
		return;
	else
	{
		
	}*/

	//SpringArmComponnt->SetWorldRotation(OffsetRotationAngles);
	SpringArmComponnt->SetWorldRotation(RotationAngles);
	CameraComponnt->SetRelativeRotation(OffsetRotationAngles);
	SpringArmComponnt->TargetArmLength = TrgtArmLength;
	

}








