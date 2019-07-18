// All rights reserved.

#include "Ammo.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Materials/MaterialInterface.h"
#include "Dustpan.h"
#include "CharacterManager.h"

// Sets default values
AAmmo::AAmmo() : Value(1), AActor(), ISuckableObjects()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UStaticMesh> Coin(TEXT("/Game/Weapons/Ammunition/SM_Soul_01.SM_Soul_01"));
	//ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));


	AmmoCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Ammo Collision"));
	AmmoCollider->SetCollisionProfileName(FName("CoinCollisionDefault"));
	AmmoCollider->SetSimulatePhysics(true);

	RootComponent = AmmoCollider;

	FVector Scale{ 1.5f,1.5f,1.5 };

	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>("Ammo Mesh");

	if (Coin.Succeeded())
	{
		AmmoMesh->SetStaticMesh(Coin.Object);
	}

	AmmoMesh->SetRelativeScale3D(Scale);
	/*if (MeshMaterial.Succeeded())
	{
		AmmoMesh->SetMaterial(0, MeshMaterial.Object);
	}*/
	AmmoMesh->SetupAttachment(RootComponent);
	AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AmmoMesh->bRenderCustomDepth = true;
	AmmoMesh->CustomDepthStencilValue = 3;

	Tags.Add("Ammo");
}

// Called when the game starts or when spawned
void AAmmo::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmo::OnBeingSucked(const UDustpan & Dustpan)
{
	FVector Location = Dustpan.GetComponentLocation();
	FVector Direction;

	//We calculate vector from me to dustpan
	Direction = Location - GetActorLocation();
	Direction.Normalize();

	//If we are close
	if (FVector::Dist(GetActorLocation(), Location) <= Dustpan.GetSuckedDistance())
	{
		Dustpan.GetPlayerManager()->GiveSomeAmmo(GetValue());

		IsSucked = true;
	}
	else
	{
		//We apply force to me
		AmmoCollider->SetPhysicsLinearVelocity(Direction * Dustpan.GetSuckVelocity());
	}
}

void AAmmo::OnObjectExitsPool()
{
	AmmoCollider->SetSimulatePhysics(true);
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	IsSucked = false;
}

void AAmmo::OnObjectEnterPool()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	AmmoCollider->SetSimulatePhysics(false);
	SetActorLocation(OwnerPool->GetActorLocation());
}
