// All rights reserved.

#include "ToxicBall.h"
#include "ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "ToxicArea.h"
#include "Twalien.h"
#include "CryptecksLevel.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "SharedLifeComponent.h"
#include "BasicWarrior.h"
#include "LifeComponent.h"

// Sets default values
AToxicBall::AToxicBall() : Damage(15), ToxicReleaseTime(1.5f), ConstructingArea(nullptr), Time(0), MoveTime(0), FallDeviation(100, 100, 500), SinHeigth(150), AActiveCrypteckBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Crypteck's ball: Toxic"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Engine/EditorMeshes/ArcadeEditorSphere.ArcadeEditorSphere"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Materials/E_Purple.E_Purple"));
	if (MeshFinder.Succeeded())
	{
		Mesh->SetStaticMesh(MeshFinder.Object);
	}
	if (MaterialFinder.Succeeded())
	{
		Mesh->SetMaterial(0, MaterialFinder.Object);
	}
}

// Called when the game starts or when spawned
void AToxicBall::BeginPlay()
{
	Super::BeginPlay();
	
}

void AToxicBall::OnBallHit(const FHitResult & Hit)
{
	//Check if we hit players
	if (Hit.GetActor() && Hit.GetActor()->Tags.Contains("Player"))
	{
		ATwalien * Player = Cast<ATwalien>(Hit.GetActor());
		if (Player)
		{
			//Translate player
			FVector Direction = Player->GetActorLocation() - GetActorLocation();
			Direction.Z = 0;
			if (Direction == FVector::ZeroVector)
			{
				UWorld * World = GetWorld();
				ACryptecksLevel * Level = Cast<ACryptecksLevel>(World->GetLevelScriptActor(World->GetCurrentLevel()));
				if (Level)
				{
					Direction = Level->GetLevelCenter() - GetActorLocation();
					Direction.Z = 0;
				}
			}
			Direction.Normalize();
			Player->SetActorLocation(Player->GetActorLocation() + (Direction * Collider->GetScaledSphereRadius() * 2.75f));

			//set new player HP
			Player->GetLifeComponent()->SetHitPoints(-Damage);
		}
	}
	else if (Hit.GetActor() && Hit.GetActor()->Tags.Contains("Enemy"))
	{
		ABasicWarrior * Ally = Cast<ABasicWarrior>(Hit.GetActor());
		if (Ally)
		{
			//Translate player
			FVector Direction = Ally->GetActorLocation() - GetActorLocation();
			Direction.Z = 0;
			if (Direction == FVector::ZeroVector)
			{
				UWorld * World = GetWorld();
				ACryptecksLevel * Level = Cast<ACryptecksLevel>(World->GetLevelScriptActor(World->GetCurrentLevel()));
				if (Level)
				{
					Direction = Level->GetLevelCenter() - GetActorLocation();
					Direction.Z = 0;
				}
			}
			Direction.Normalize();
			Ally->SetActorLocation(Ally->GetActorLocation() + (Direction * Collider->GetScaledSphereRadius() * 2.75f));

			//set new player HP
			Ally->GetLifeComponent()->SetHitPoints(-Damage);
		}
	}
	else if (Hit.GetActor() && Hit.GetActor()->Tags.Contains("Crypteck's Sting"))
	{
		Hit.GetActor()->Destroy();
	}
	else if (Hit.GetActor() && Hit.GetActor()->GetName().Contains("ForceField"))
	{
		State = DISSOLVE_PRE_RETURN;
	}
}

void AToxicBall::CreateOrbitNewPosition(FVector & Position)
{
	float Sin, Cos;
	FMath::SinCos(&Sin, &Cos, FMath::DegreesToRadians(Angle));
	Position.Set(OrbitCenter.X + (Cos * WidthSpace), OrbitCenter.Y + (Sin * HeightSpace), OrbitCenter.Z + (Cos * HeightSpace));
}

void AToxicBall::Move(float DeltaSeconds)
{
	if (MoveTime < 1.f)
	{
		if (MoveTime < 1 && MoveTime + DeltaSeconds >= 1.f)
		{
			MoveTime = 1.f;
		}
		else
		{
			MoveTime += DeltaSeconds;
		}

		SetActorLocation(FVector(FMath::Lerp(TeleportPosition.X, TargetPosition.X, MoveTime), FMath::Lerp(TeleportPosition.Y, TargetPosition.Y, MoveTime), FallDeviation.Z + (FMath::Sin(MoveTime * 3.141592f) * SinHeigth) ));
	}
	else
	{
		FHitResult Hit;
		FVector Displacement = TargetPosition - GetActorLocation();
		FVector Movement = DisplacementNormal * Speed * DeltaSeconds;

		if (Movement.SizeSquared() >= Displacement.SizeSquared())
		{
			Movement = TargetPosition;
			State = ONACTION;
		}
		else
		{
			Movement += GetActorLocation();
		}

		SetActorLocation(Movement, true, &Hit, ETeleportType::None);

		if (Hit.bBlockingHit && Hit.GetActor())
		{
			OnBallHit(Hit);
		}
	}
}

void AToxicBall::SetPositionToAttackTeleport()
{
	UWorld * World = GetWorld();
	ACryptecksLevel * Level = Cast<ACryptecksLevel>(World->GetLevelScriptActor(World->GetCurrentLevel()));
	if (Level)
	{
		TargetPosition.Z += 80;
		FVector Aux = Level->GetLevelCenter() - TargetPosition;

		if (Aux.X > 0)
			Aux.X = FallDeviation.X;
		else if (Aux.X < 0)
			Aux.X = -FallDeviation.X;

		if (Aux.Y > 0)
			Aux.Y = FallDeviation.Y;
		else if (Aux.Y < 0)
			Aux.Y = -FallDeviation.Y;

		Aux.Z = FallDeviation.Z;

		TeleportPosition = TargetPosition + Aux;
		DisplacementNormal = FVector(0, 0, -1);
		MoveTime = 0.f;
	}
}

// Called every frame
void AToxicBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AToxicBall::Action(float DeltaSeconds)
{
	if (!ConstructingArea)
	{
		ConstructingArea = GetWorld()->SpawnActor<AToxicArea>(GetActorLocation(), FRotator(0, 0, 0), FActorSpawnParameters());
	}
	Time += DeltaSeconds;
	ConstructingArea->OnConstruct(Time / ToxicReleaseTime);
	if (Time >= ToxicReleaseTime)
	{
		FVector Displacement = TargetPosition - GetActorLocation();
		DisplacementNormal = Displacement.GetSafeNormal();
		State = DISSOLVE_PRE_RETURN;
		Time = 0;
		ConstructingArea = nullptr;
	}
}

