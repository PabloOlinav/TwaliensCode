// All rights reserved.

#include "BurningArea.h"
#include "ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"

// Sets default values
ABurningArea::ABurningArea() : ACrypteckArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Engine/EngineMeshes/Sphere.Sphere"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Engine/EngineDebugMaterials/M_SimpleTranslucent.M_SimpleTranslucent"));
	if (MeshFinder.Succeeded())
	{
		Mesh->SetStaticMesh(MeshFinder.Object);
	}
	if (MaterialFinder.Succeeded())
	{
		Mesh->SetMaterial(0, MaterialFinder.Object);
	}
	RootComponent->SetWorldScale3D(FVector(5.f, 5.f, 5.f));

	MaximumDPS = 25.f;
	DPS = 0;
	Tags.Add("Crypteck's burning area");
}

// Called when the game starts or when spawned
void ABurningArea::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABurningArea::OnSpecificConstruct(float Percentage)
{

}

// Called every frame
void ABurningArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

