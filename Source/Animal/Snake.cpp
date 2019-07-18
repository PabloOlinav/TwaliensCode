// All rights reserved.

#include "Snake.h"
#include "Components/SkeletalMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "AnimalController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASnake::ASnake()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Edit SKeletal Mesh Component
	USkeletalMeshComponent * MyMesh = GetMesh();
	MyMesh->SetRelativeLocation(FVector(6.5f, 1.5f, 0));
	MyMesh->SetRelativeRotation(FRotator(0, -90, 0).Quaternion());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshFinder(TEXT("/Game/Environment/Animals/SK_Snake_01"));
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimFinder(TEXT("/Game/Animations/Animals/Snake/Snake_AnimBP"));
	if (MeshFinder.Succeeded())
	{
		MyMesh->SetSkeletalMesh(MeshFinder.Object);
	}
	MyMesh->SetCollisionProfileName(FName("NoCollision"));
	MyMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	if (AnimFinder.Succeeded())
	{
		MyMesh->SetAnimInstanceClass(AnimFinder.Class);
	}

	//Edit Capsule Component
	UCapsuleComponent * MyCapsuleComponent = GetCapsuleComponent();
	MyCapsuleComponent->SetCapsuleRadius(4.f);
	MyCapsuleComponent->SetCapsuleHalfHeight(4.f);
	MyCapsuleComponent->SetCollisionProfileName("IgnoreOnlyPawn");

	//Edit Movement component
	GetCharacterMovement()->MaxWalkSpeed = 60;

	//Set Preferred AI Controller
	AIControllerClass = AAnimalController::StaticClass();
}

int ASnake::GetRegionIndex() const
{
	return RegionIndex;
}

void ASnake::SetRegionIndex(int NewRegionIndex)
{
	RegionIndex = NewRegionIndex;
}

// Called when the game starts or when spawned
void ASnake::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASnake::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

