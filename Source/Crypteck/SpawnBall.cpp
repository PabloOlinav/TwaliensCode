// All rights reserved.

#include "SpawnBall.h"
#include "ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "BasicWarriorController.h"
#include "CharacterManager.h"

// Sets default values
ASpawnBall::ASpawnBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Crypteck's ball: Spawn"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Engine/EditorMeshes/ArcadeEditorSphere.ArcadeEditorSphere"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Materials/Mat_Water.Mat_Water"));
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
void ASpawnBall::BeginPlay()
{
	Super::BeginPlay();
	
	TActorIterator<ACharacterManager> PMIterator(GetWorld());
	PlayerManager = *PMIterator;
}

void ASpawnBall::OnBallHit(const FHitResult & Hit)
{}

void ASpawnBall::CreateOrbitNewPosition(FVector & Position)
{}

void ASpawnBall::SetPositionToAttackTeleport()
{
}

// Called every frame
void ASpawnBall::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	if (State == DISSOLVE_PRE_GOTO_ORBIT)
	{
		State = IDLE;
	}
	if (State == DISSOLVE_PRE_ATTACK)
	{
		Action(DeltaTime);
		State = IDLE;
	}
}

void ASpawnBall::Action(float DeltaSeconds)
{
	TArray<ABasicWarrior *> Enemies;
	int i = 0;
	for(AEnemySpawner * Spawner : SpawnersLinkedToSpawnBall)
	{
		Spawner->SpawnEnemyWithReference(Enemies);
	}

	for(ABasicWarrior * Enemy : Enemies)
	{
		ABasicWarriorController * Controller = Cast<ABasicWarriorController>(Enemy->GetController());

		if (Controller)
		{
			Controller->SetAgentTarget(PlayerManager->GetPlayer(i));

			if (i == 0)
			{
				i = 1;
			}
			else
			{
				i = 0;
			}
		}
	}
}

