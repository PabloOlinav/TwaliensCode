// All rights reserved.

#include "Spider.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "ConstructorHelpers.h"
#include "Animation/AnimInstance.h"
#include "AnimalController.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASpider::ASpider() : RegionIndex(0)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Edit SKeletal Mesh Component
	USkeletalMeshComponent * MyMesh = GetMesh();
	MyMesh->SetRelativeLocation(FVector(0, 0, -17.f));
	MyMesh->SetRelativeRotation(FRotator(0, -90, 0).Quaternion());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshFinder(TEXT("/Game/Environment/Animals/SK_Spider_01"));
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimFinder(TEXT("/Game/Animations/Animals/Spider/Spider_AnimBP"));
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
void ASpider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
