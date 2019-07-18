// All rights reserved.

#include "Bunny.h"
#include "Components/SkeletalMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "AnimalController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABunny::ABunny()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Edit SKeletal Mesh Component
	USkeletalMeshComponent * MyMesh = GetMesh();
	MyMesh->SetRelativeLocation(FVector(0, 0, -17.f));
	MyMesh->SetRelativeRotation(FRotator(0, -90, 0).Quaternion());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshFinder(TEXT("/Game/Environment/Animals/SK_Bunny_01"));
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimFinder(TEXT("/Game/Animations/Animals/Bunny/Bunny_AnimBP"));
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
	MyCapsuleComponent->SetCapsuleRadius(15.f);
	MyCapsuleComponent->SetCapsuleHalfHeight(17.f);
	MyCapsuleComponent->SetCollisionProfileName("IgnoreOnlyPawn");

	//Edit Movement component
	GetCharacterMovement()->MaxWalkSpeed = 75;

	//Set Preferred AI Controller
	AIControllerClass = AAnimalController::StaticClass();
}

// Called when the game starts or when spawned
void ABunny::BeginPlay()
{
	Super::BeginPlay();

	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

// Called every frame
void ABunny::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABunny::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABunny::StopMovement()
{
	GetCharacterMovement()->MaxWalkSpeed = 0;
}

void ABunny::StartMovement()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

