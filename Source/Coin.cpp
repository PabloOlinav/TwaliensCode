// All rights reserved.

#include "Coin.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Materials/MaterialInterface.h"
#include "ConstructorHelpers.h"

// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UStaticMesh> Coin(TEXT("/Engine/BasicShapes/Cylinder"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
		

	CoinCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Coin Trigger"));
	CoinCollider->SetCollisionProfileName(FName("CoinCollisionDefault"));
	CoinCollider->SetSimulatePhysics(true);
	RootComponent = CoinCollider;

	FVector Scale{.7f,.7f,0.05};

	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>("Coin Mesh");
	if(Coin.Succeeded())
	{
		CoinMesh->SetStaticMesh(Coin.Object);
	}
	CoinMesh->SetRelativeScale3D(Scale);
	if(MeshMaterial.Succeeded())
	{
		CoinMesh->SetMaterial(0, MeshMaterial.Object);
	}
	CoinMesh->SetupAttachment(RootComponent);

	Tags.Add("Coin");
}

// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ACoin::GetValue()
{
	return value;
}

void ACoin::SetValue(float NewValue)
{
	value = NewValue;
}

UPrimitiveComponent* ACoin::GetCollider()
{
	return CoinCollider;
}
