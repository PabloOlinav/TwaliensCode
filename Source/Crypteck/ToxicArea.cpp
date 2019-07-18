// All rights reserved.

#include "ToxicArea.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/CapsuleComponent.h"
#include "Twalien.h"
#include "SharedLifeComponent.h"
#include "CryptecksLevel.h"

// Sets default values
AToxicArea::AToxicArea() : ACrypteckArea(), Level(nullptr), Constructed(false), DrainTimeModifier(.2f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Engine/EngineMeshes/Sphere.Sphere"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Engine/VREditor/UI/KeyMaterial.KeyMaterial"));
	if (MeshFinder.Succeeded())
	{
		Mesh->SetStaticMesh(MeshFinder.Object);
	}
	if(MaterialFinder.Succeeded())
	{
		Mesh->SetMaterial(0, MaterialFinder.Object);
	}
	RootComponent->SetWorldScale3D(FVector(1.25f, 1.25f, 1.25f));

	MaximumDPS = 15.f;
	DPS = 0;
	Tags.Add("Crypteck's toxic area");
}

// Called when the game starts or when spawned
void AToxicArea::BeginPlay()
{
	Super::BeginPlay();

	UWorld * World = GetWorld();
	ACryptecksLevel * MyLevel = Cast<ACryptecksLevel>(World->GetLevelScriptActor(World->GetCurrentLevel()));
	if (MyLevel)
	{
		Level = MyLevel;
	}
}

void AToxicArea::OnSpecificConstruct(float Percentage)
{
	ConstructedPercentage = Percentage;
	Constructed = Percentage == 1;
}

// Called every frame
void AToxicArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AToxicArea::Drain(float DeltaSeconds)
{
	bool Drained;
	
	ConstructedPercentage -= DeltaSeconds * DrainTimeModifier;
	Drained = ConstructedPercentage <= 0;

	if (Drained)
	{
		if (Level)
		{
			Level->ReturnInnerCell(GetActorLocation());
		}
	}

	return Drained;
}

void AToxicArea::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AToxicArea::OnBeingSucked(const class UDustpan & Dustpan)
{
	if (Drain(GetWorld()->GetDeltaSeconds()))
	{
		IsSucked = true;
	}
}
