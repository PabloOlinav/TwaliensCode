// All rights reserved.

#include "SmashBall.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMesh.h"
#include "ConstructorHelpers.h"
#include "LifeComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "CharacterManager.h"
#include "Twalien.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CryptecksLevel.h"
#include "SharedLifeComponent.h"
#include "BasicWarrior.h"

// Sets default values
ASmashBall::ASmashBall() : RestTime(3.f), Damage(15), AActiveCrypteckBall()
{
	//Super::AActiveCrypteckBall();
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Crypteck's ball: Smash"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Engine/EditorMeshes/ArcadeEditorSphere.ArcadeEditorSphere"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Materials/M_Blue.M_Blue"));
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
void ASmashBall::BeginPlay()
{
	Super::BeginPlay();
}

void ASmashBall::OnBallHit(const FHitResult & Hit)
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

void ASmashBall::CreateOrbitNewPosition(FVector & Position)
{
	float Sin, Cos;
	FMath::SinCos(&Sin, &Cos, FMath::DegreesToRadians(Angle));
	Position.Set(OrbitCenter.X + (Cos * WidthSpace), OrbitCenter.Y + (Sin * WidthSpace), OrbitCenter.Z);
}

void ASmashBall::Move(float DeltaSeconds)
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

void ASmashBall::SetPositionToAttackTeleport()
{
	TeleportPosition = TargetPosition + (FVector(0, 0, 1000));
	DisplacementNormal = FVector(0, 0, -1);
}

void ASmashBall::ActionEnded()
{
	State = DISSOLVE_PRE_RETURN;
}

// Called every frame
void ASmashBall::Tick(float DeltaTime)
{
	//Al logic is in parent's tick
	Super::Tick(DeltaTime);
}

void ASmashBall::Action(float DeltaSeconds)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(ActionTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &ASmashBall::ActionEnded, 1, false, RestTime);
	}
}