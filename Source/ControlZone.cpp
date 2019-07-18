// Fill out your copyright notice in the Description page of Project Settings.

#include "ControlZone.h"
#include "Components/SphereComponent.h"
#include "Twalien.h"
#include "Dustpan.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "PileFragment.h"
#include "Engine/TargetPoint.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "EnemySpawner.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AControlZone::AControlZone() : Super(), TotalInFragmentAmount(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ControlArea = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	ControlArea->SetSphereRadius(264.f);
	ControlArea->SetCollisionProfileName(FName("OverlapAllDynamic"));
	RootComponent = ControlArea;

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Game/Environment/Altar/SM_AltarBase_01"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Environment/Materials/M_AltarBase_01"));
	ControlMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ControlZoneMesh"));
	ControlMesh->SetRelativeLocation(FVector::ZeroVector);
	ControlMesh->SetRelativeRotation(FRotator::ZeroRotator);
	ControlMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 0.05f));

	if (MeshFinder.Succeeded())
	{
		ControlMesh->SetStaticMesh(MeshFinder.Object);
	}
	if (MaterialFinder.Succeeded())
	{
		ControlMesh->SetMaterial(0, MaterialFinder.Object);
	}
	ControlMesh->SetCollisionProfileName(FName("NoCollision"));
	ControlMesh->SetupAttachment(ControlArea);

	// Particles

			PS_ControlZone = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PS_ControlZn"));
			PS_ControlZone->SetupAttachment(RootComponent);
			ConstructorHelpers::FObjectFinder<UParticleSystem> CZFX(TEXT("/Game/Effects/Particles/PS_AltarLight_01"));
	
			if (CZFX.Succeeded())
			{
				PS_ControlZone->SetTemplate(CZFX.Object);
			}

	//PS_ControlZone->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AControlZone::BeginPlay()
{
	Super::BeginPlay();

	//If this fails, someone probably didnt do their job
	check(RelicsLocations.Num() > 0);
	//If this fails probably you should increase target points in Relics Locations
	check(RelicsBoundToAltar.Num() <= RelicsLocations.Num());

	RelicsBoundToAltar.Reserve(RelicsLocations.Num());

	if (Spawners.Num() > 0)
	{
		ActivateSpawners();
	}
	bCanglow = true;
}

// Called every frame
void AControlZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		if (!PS_ControlZone->bWasDeactivated && !bCanglow)
			return;

		else if (!PS_ControlZone->bWasDeactivated && bCanglow)
		{
			PS_ControlZone->ActivateSystem(true);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
			bCanglow = false;
			//PS_ControlZone->DeactivateSystem();
		}

	
		
}

void AControlZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	APileFragment* Pile = Cast<APileFragment>(OtherActor);
	FVector Height;
	FVector Center;
	int Index;

	if(Pile)
	{
		if (RelicsBoundToAltar.Contains(Pile)) 
		{
			Index = TotalInFragmentAmount++;
			Height = Pile->GetFragmentMesh()->GetStaticMesh()->GetBounds().BoxExtent;
			Pile->SetActorLocation(RelicsLocations[Index]->GetActorLocation() + RelicsLocations[Index]->GetActorUpVector() * Height);
			Pile->OnItemBecomeDisable();
			OnFragmentCome.Broadcast();

			if (TotalInFragmentAmount == RelicsLocations.Num())
			{
				DeactivateSpawners();
				PS_ControlZone->DeactivateSystem();
			}
				
		}
	}
}

void AControlZone::ActivateSpawners()
{
	for (AEnemySpawner * Spawner : Spawners)
	{
		Spawner->SetIsActivated(true);
	}
}

void AControlZone::DeactivateSpawners()
{
	for (AEnemySpawner * Spawner : Spawners)
	{
		Spawner->SetIsActivated(false);
	}
}

void AControlZone::AddObserverToProgress(FScriptDelegate Delegate)
{
	OnFragmentCome.Add(Delegate);
}

void AControlZone::RemoveObserverFromProgress(FScriptDelegate Delegate)
{
	OnFragmentCome.Remove(Delegate);
}

void AControlZone::ClearObservers()
{
	OnFragmentCome.Clear();
}

int AControlZone::GetTotalFragmentAmount()
{
	return RelicsLocations.Num();
}

void AControlZone::BindFragmentToAltar(class APileFragment * Fragment)
{
	//If this fails probably you should increase target points in Relics Locations
	check(RelicsBoundToAltar.Num() < RelicsLocations.Num());

	RelicsBoundToAltar.Add(Fragment);
}

void AControlZone::UnbindFragmentToAltar(class APileFragment * Fragment)
{
	const int Index = RelicsBoundToAltar.Find(Fragment);
	const int LastIndex = RelicsBoundToAltar.Num() - 1;

	RelicsBoundToAltar[Index] = RelicsBoundToAltar[LastIndex];
	RelicsBoundToAltar[LastIndex] = Fragment;
	RelicsBoundToAltar.RemoveAt(LastIndex);
}
