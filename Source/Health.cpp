// All rights reserved.

#include "Health.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LifeFunctionality.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Materials/MaterialInterface.h"


// Sets default values
AHealth::AHealth()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Game/Items/SM_HealthItem_01"));
	//ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

	HealthCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Health Collider"));
	HealthCollider->SetCollisionProfileName(FName("AimingAssist"));
	RootComponent = HealthCollider;

	FVector Scale{ 1.0f,1.0f,1.0f };

	HealthMesh = CreateDefaultSubobject<UStaticMeshComponent>("Health Mesh");

	if (Cube.Succeeded())
	{
		HealthMesh->SetStaticMesh(Cube.Object);
		HealthMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		HealthMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}

	//HealthMesh->SetRelativeScale3D(Scale);

	//if (MeshMaterial.Succeeded())
	//{
	//	HealthMesh->SetMaterial(0, MeshMaterial.Object);
	//}

	HealthMesh->SetupAttachment(RootComponent);
	HealthMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Tags.Add("Health");

	AllowedTagsToInteract.Add(FName("Player"));
}

// Called when the game starts or when spawned
void AHealth::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealth::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	bool IsAllowed = false;

	for (int i = 0; i < OtherActor->Tags.Num() && !IsAllowed; i++)
	{
		IsAllowed = AllowedTagsToInteract.Contains(OtherActor->Tags[i]);
	}

	if (OtherActor->Tags.Contains("Player"))
	{
		TSet<UActorComponent *> Components = OtherActor->GetComponents();
		ILifeFunctionality * LifeComponent = nullptr;

		for (UActorComponent * Component : Components)
		{
			LifeComponent = Cast<ILifeFunctionality>(Component);
			if (LifeComponent)
			{
				break;
			}
		}

		if (LifeComponent)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Health Recharge"));
			LifeComponent->SetHitPoints(HealthAmount);
		}

		this->Destroy();
	}
}