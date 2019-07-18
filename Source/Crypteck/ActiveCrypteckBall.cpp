// All rights reserved.

#include "ActiveCrypteckBall.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "CharacterManager.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Crypteck.h"

// Sets default values
AActiveCrypteckBall::AActiveCrypteckBall() : DeltaTimeMultiplier(0.33f), Speed(2000), WidthSpace(350.f), HeightSpace(200), InitialAngle(0), CurrentOrbitTime(0),
DissolveFinished(false), Angle(0), ICrypteckBall(), AActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("ActiveCrypteckBallCollider"));
	Collider->SetSphereRadius(160);
	Collider->SetNotifyRigidBodyCollision(true);
	Collider->SetCollisionProfileName(FName("UnderGroundPhysicsActor"));
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActiveCrypteckBallMesh"));
	Mesh->SetCollisionProfileName(FName("NoCollision"));
	Mesh->SetupAttachment(RootComponent);

	Life = CreateDefaultSubobject<ULifeComponent>(TEXT("ActiveCrypteckBallLife"));
	DeathDelegate.BindUFunction(this, FName("OnDeath"));
	Life->AddDelegateToDeath(DeathDelegate);
	AddOwnedComponent(Life);
}

// Called when the game starts or when spawned
void AActiveCrypteckBall::BeginPlay()
{
	Super::BeginPlay();
}

void AActiveCrypteckBall::Dissolve(float DeltaSeconds)
{
	DissolveFinished = true;
}

void AActiveCrypteckBall::Undissolve(float DeltaSeconds)
{
	DissolveFinished = true;
}

void AActiveCrypteckBall::Orbit(float DeltaSeconds)
{
	FVector Location = OrbitCenter, NewPosition;
	FHitResult Hit;
	CurrentOrbitTime += DeltaTimeMultiplier * DeltaSeconds;
	Angle = InitialAngle + (CurrentOrbitTime * 360);
	CreateOrbitNewPosition(NewPosition);
	SetActorLocation(NewPosition, true, &Hit, ETeleportType::None);

	if (Hit.bBlockingHit && Hit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit %s with %s when orbiting."), *GetName(), *Hit.GetActor()->GetName());
	}
}

void AActiveCrypteckBall::Teleport(float DeltaSeconds)
{
	SetActorLocation(TeleportPosition);
}

void AActiveCrypteckBall::OnDeath()
{
	Destroy();
}

void AActiveCrypteckBall::ConvertAngleToOrbitTime(float NewAngle)
{
	CurrentOrbitTime = (NewAngle - InitialAngle) / 360;
}

// Called every frame
void AActiveCrypteckBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float NewAngle;

	switch (State)
	{
		case DISSOLVE_PRE_GOTO_ORBIT:
			Dissolve(DeltaTime);
			if (DissolveFinished)
			{
				State = TELEPORT_TO_ORBIT;
				DissolveFinished = false;
			}
		break;
		case TELEPORT_TO_ORBIT:
			NewAngle = Crypteck->AssignFreeAngle(this);
			ConvertAngleToOrbitTime(NewAngle);
			Angle = NewAngle;
			CreateOrbitNewPosition(TeleportPosition);
			Teleport(DeltaTime);
			if (GetActorLocation() == TeleportPosition)
			{
				State = IDLE;
				Crypteck->OnBallBeginOrbit(this);
			}
		break;
		case IDLE:
			Orbit(DeltaTime);
			Undissolve(DeltaTime);
		break;
		case DISSOLVE_PRE_ATTACK:
			Dissolve(DeltaTime);
			if (DissolveFinished)
			{
				State = TELEPORT_TO_ATTACK;
				DissolveFinished = false;
			}
		break;
		case TELEPORT_TO_ATTACK:
			Teleport(DeltaTime);
			if (GetActorLocation() == TeleportPosition)
			{
				State = DISSOLVE_POST_ATTACK;
			}
		break;
		case DISSOLVE_POST_ATTACK:
			Undissolve(DeltaTime);
			if (DissolveFinished)
			{
				State = ONTHEWAY;
				DissolveFinished = false;
			}
		break;
		case ONTHEWAY:
			Move(DeltaTime);
		break;
		case ONACTION:
			Action(DeltaTime);
		break;
		case DISSOLVE_PRE_RETURN:
			Dissolve(DeltaTime);
			if (DissolveFinished)
			{
				State = RETURNING;
				DissolveFinished = false;
			}
		break;
		case RETURNING:
			NewAngle = Crypteck->AssignFreeAngle(this);
			ConvertAngleToOrbitTime(NewAngle);
			Angle = NewAngle;
			CreateOrbitNewPosition(TeleportPosition);
			Teleport(DeltaTime);
			if (GetActorLocation() == TeleportPosition)
			{
				State = IDLE;
				Crypteck->OnBallBeginOrbit(this);
			}
		break;
		default:
		break;
	}
}

void AActiveCrypteckBall::SetTarget(const FVector & Target)
{	
	if (State == BALL_STATE::IDLE)
	{
		State = DISSOLVE_PRE_ATTACK;
		TargetPosition = Target;
		SetPositionToAttackTeleport();
	}
}

void AActiveCrypteckBall::Activate()
{
	if (State == DEACTIVATED)
	{
		State = DISSOLVE_PRE_GOTO_ORBIT;
	}
}

void AActiveCrypteckBall::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Life->RemoveAllDelegatesFromDeath();
	Life->RemoveAllDelegatesFromDeath();
}

