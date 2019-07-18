// All rights reserved.

#include "BasicWarrior.h"
#include "Engine/Engine.h"
#include "ConstructorHelpers.h"
#include "LifeComponent.h"
#include "LifeFunctionality.h"
#include "AIController.h"
#include "CoinLoot.h"
#include "Components/SkeletalMeshComponent.h"
#include "ParticlesSceneComponent.h"
#include "PoolManager.h"
#include "BasicWarriorController.h"
#include "Dustpan.h"
#include "CharacterManager.h"
#include "BrainComponent.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Materials/MaterialInterface.h"
#include "Animation/AnimInstance.h"
#include "Structure/BasicWarriorAnimationData.h"
#include "FMODEvent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABasicWarrior::ABasicWarrior() : Damage(8), MeleeDistance(150), Value(4), AnimationData(), DissolveCurrentTime(0), DissolveTotalTime(.575f), ISuckableObjects()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	static ConstructorHelpers::FClassFinder<AActor> Coins(TEXT("/Game/Weapons/Ammunition/BP_Coins"));

	PrimaryActorTick.bCanEverTick = true;

	//Initializing components.	

	//FMod initialization
	IFMODStudioModule::Get();

	//HP Component
	HPComponent = CreateDefaultSubobject<ULifeComponent>("LifeComponent");
	AddOwnedComponent(HPComponent);

	DeadDelegate.BindUFunction(this, FName("OnLogicDeath"));
	HPComponent->AddDelegateToDeath(DeadDelegate);

	DamageDelegate.BindUFunction(this, FName("OnHealthChanged"));
	HPComponent->AddDelegateToHPUpdate(DamageDelegate);

	CoinLootComponent = CreateDefaultSubobject<UCoinLoot>("CoinLootComponent");
	AddOwnedComponent(CoinLootComponent);

	//Skeletal Mesh component
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshFinder(TEXT("/Game/Characters/Enemies/SK_Enemy_01"));
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBpClass(TEXT("/Game/Animations/Enemies/Enemy_01_AnimBP"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> Material1Finder(TEXT("/Game/Characters/Materials/M_EnemyDissolve_01"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> Material2Finder(TEXT("/Game/Characters/Materials/MI_EnemyWPDissolve_01"));

	USkeletalMeshComponent * MyMesh = GetMesh();
	
	MyMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	if (AnimBpClass.Succeeded())
	{
		MyMesh->SetAnimInstanceClass(AnimBpClass.Class);
	}

	if (MeshFinder.Succeeded())
	{
		MyMesh->SetSkeletalMesh(MeshFinder.Object);
	}

	if (Material1Finder.Succeeded())
	{
		MyMesh->SetMaterial(0, Material1Finder.Object);
	}

	if (Material2Finder.Succeeded())
	{
		MyMesh->SetMaterial(1, Material2Finder.Object);
	}

	MyMesh->SetCollisionProfileName(FName("NoCollision"));

	MyMesh->bRenderCustomDepth = true;
	MyMesh->CustomDepthStencilValue = 1;

	//Adding left melee attack trigger
	LeftPunchTrigger = CreateDefaultSubobject<UBoxComponent>("LeftHandPunchTrigger");
	LeftPunchTrigger->SetupAttachment(GetMesh(), FName("hand_slide_l"));

	LeftPunchTrigger->SetRelativeLocation(FVector(65, 0, 0));
	LeftPunchTrigger->SetRelativeScale3D(FVector(1, 0.25f, 0.25f));
	LeftPunchTrigger->SetCollisionProfileName(FName("Custom..."));
	LeftPunchTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftPunchTrigger->SetCollisionObjectType(ECC_WorldDynamic);
	LeftPunchTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeftPunchTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	PunchOverlapDelegate.BindUFunction(this, FName("OnPunchBeginOverlap"));
	LeftPunchTrigger->OnComponentBeginOverlap.Add(PunchOverlapDelegate);

	//Adding right melee attack trigger
	RightPunchTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandPunchTrigger"));
	RightPunchTrigger->SetupAttachment(GetMesh(), FName("hand_slide_r"));

	RightPunchTrigger->SetRelativeLocation(FVector(65, 0, 0));
	RightPunchTrigger->SetRelativeScale3D(FVector(1, 0.25f, 0.25f));
	RightPunchTrigger->SetCollisionProfileName(FName("Custom..."));
	RightPunchTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightPunchTrigger->SetCollisionObjectType(ECC_WorldDynamic);
	RightPunchTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	RightPunchTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	RightPunchTrigger->OnComponentBeginOverlap.Add(PunchOverlapDelegate);

	//Modifying capsule component settings
	UCapsuleComponent * Collider = GetCapsuleComponent();
	Collider->SetCapsuleHalfHeight(120.f);
	Collider->SetCapsuleRadius(27.f);

	MyMesh->SetRelativeLocation(FVector(0.f, 0.f, -120.f));
	MyMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f).Quaternion());

	//Other stuff
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	ConstructorHelpers::FClassFinder<AController> ControllerClassFinder(TEXT("/Game/Characters/BasicWarrior/BP_BasicWarriorController_0"));

	if (ControllerClassFinder.Succeeded())
	{
		AIControllerClass = ControllerClassFinder.Class;
	}

	AudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Enemy Sound"));
	ConstructorHelpers::FObjectFinder<UFMODEvent> AudioEventFinder(TEXT("/Game/Sounds/Events/Enemy/EnemySpawn"));
	if (AudioEventFinder.Succeeded())
	{
		AudioComponent->SetEvent(AudioEventFinder.Object);
	}
	AudioComponent->AttenuationDetails.bOverrideAttenuation = true;
	AudioComponent->AttenuationDetails.MinimumDistance = 30.f;
	AudioComponent->AttenuationDetails.MaximumDistance = 100.f;
	AudioComponent->SetupAttachment(GetMesh());

	Tags.Add("Enemy");

	DefaultMeshLocation = FVector(0.0f, 0.0f, -88.0f);
	DefaultMeshRotator = FRotator(0.0f, -90.0f, 0.0f);

	IsSkillAffected = true;

	// Particles
	DamageEffectComp = CreateDefaultSubobject<UParticlesSceneComponent>(TEXT("Damage Effect"));
	DamageEffectComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UParticleSystem>FloatingFX(TEXT("/Game/Effects/Particles/PS_EnemyFloat_01"));
	PS_FloatingComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Floating Effect"));
	PS_FloatingComp->SetupAttachment(RootComponent);

	if (FloatingFX.Succeeded())
	{
		PS_FloatingComp->SetTemplate(FloatingFX.Object);
	}

	GetCharacterMovement()->MaxWalkSpeed = 560.f;

	PlayersHit.Reserve(2);
	PlayersHit.Add(nullptr);
	PlayersHit.Add(nullptr);
}

void ABasicWarrior::OnBeingSucked(const class UDustpan & Dustpan)
{
	//TODO: RAGDOLLING THE ENEMY SO THE DUSTPAN CAN SUCK IT
	Dustpan.GetPlayerManager()->GiveSomeAmmo(GetValue());
	
	IsSucked = true;
}

void ABasicWarrior::MakeDamageHandsDamage(AActor * OtherActor)
{
	if (OtherActor->Tags.Contains("Player"))
	{
		ATwalien* HitPlayer = Cast<ATwalien>(OtherActor);

		if (HitPlayer && HitPlayer->GetHittedAudioComponent()->Event.Get())
		{
			HitPlayer->GetHittedAudioComponent()->Play();
		}

		if (this->PlayersHit.Contains(OtherActor))
		{
			return;
		}

		if (!this->PlayersHit[0])
		{
			this->PlayersHit[0] = OtherActor;
		}
		else
		{
			this->PlayersHit[1] = OtherActor;
		}

		TSet<UActorComponent*> Components;

		ILifeFunctionality * Life = nullptr;

		Components = OtherActor->GetComponents();

		for (UActorComponent* Component : Components)
		{
			Life = Cast<ILifeFunctionality>(Component);

			if (Life)
			{
				break;
			}
		}

		if (Life)
		{
			Life->SetHitPoints(-Damage);
		}
	}
}

void ABasicWarrior::DoDissolveDisappear(float DeltaSeconds)
{
	const FName ParamName("Dissolve");

	DissolveCurrentTime += DeltaSeconds;
	if (DissolveCurrentTime < DissolveTotalTime)
	{
		float Percentage = DissolveCurrentTime / DissolveTotalTime;
		BodyDissolveMaterial->SetScalarParameterValue(ParamName, Percentage);
		BladesDissolveMaterial->SetScalarParameterValue(ParamName, Percentage);
	}
	else
	{
		BodyDissolveMaterial->SetScalarParameterValue(ParamName, 1);
		BladesDissolveMaterial->SetScalarParameterValue(ParamName, 1);
		SetActorTickEnabled(false);
	}
}

void ABasicWarrior::DoDissolveAppear(float DeltaSeconds)
{
	const FName ParamName("Dissolve");

	DissolveCurrentTime += DeltaSeconds;
	if (DissolveCurrentTime < DissolveTotalTime)
	{
		float Percentage = DissolveCurrentTime / DissolveTotalTime;
		BodyDissolveMaterial->SetScalarParameterValue(ParamName, 1 - Percentage);
		BladesDissolveMaterial->SetScalarParameterValue(ParamName, 1 - Percentage);
	}
	else
	{
		BodyDissolveMaterial->SetScalarParameterValue(ParamName, 0);
		BladesDissolveMaterial->SetScalarParameterValue(ParamName, 0);
		SetActorTickEnabled(false);
	}
}

// Called when the game starts or when spawned
void ABasicWarrior::BeginPlay()
{
	Super::BeginPlay();

	//PS_FloatingComp->ActivateSystem(true);

	LastHP = HPComponent->GetHitPoints();

	BodyDissolveMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	GetMesh()->SetMaterial(0, BodyDissolveMaterial);

	BladesDissolveMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), this);
	GetMesh()->SetMaterial(1, BladesDissolveMaterial);

	ActivateDissolveAppear();
}

// Called every frame
void ABasicWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PS_FloatingComp->bWasDeactivated)
		PS_FloatingComp->ActivateSystem(true);

	if (DissolveAppear)
	{
		DoDissolveAppear(DeltaTime);
	}
	else
	{
		DoDissolveDisappear(DeltaTime);
	}
}

// Called to bind functionality to input
void ABasicWarrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasicWarrior::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	HPComponent->UnregisterComponent();
	HPComponent->DestroyComponent();
}

bool ABasicWarrior::GetIsAttacking()
{
	return AnimationData.IsUsingTheirWeapon;
}

void ABasicWarrior::SetIsAttacking(bool NewAttackState)
{
	AnimationData.IsUsingTheirWeapon = NewAttackState;

	if (!AnimationData.IsUsingTheirWeapon)
	{
		DeactivatePunchDamage();
	}
}

float ABasicWarrior::GetAttackDistance()
{
	return MeleeDistance;
}

void ABasicWarrior::ActivatePunchDamage()
{
	LeftPunchTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightPunchTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABasicWarrior::DeactivatePunchDamage()
{
	LeftPunchTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightPunchTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PlayersHit[0] = nullptr;
	PlayersHit[1] = nullptr;
}

void ABasicWarrior::OnPunchBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	MakeDamageHandsDamage(OtherActor);
}

void ABasicWarrior::OnHealthChanged()
{
	//Get current hp
	float CurrentHP = HPComponent->GetHitPoints();
	//if we have less health, we receive damage
	if (CurrentHP < LastHP)
	{
		AnimationData.HasTakenDamage = true;
	}
	//update HP
	LastHP = CurrentHP;
}

void ABasicWarrior::OnDeath()
{
	CoinLootComponent->SpawnCoins();

	SetActorHiddenInGame(true);

	if (OwnerPool)
	{
		ABasicWarriorController* MyController = Cast<ABasicWarriorController>(this->GetController());

		ReturnToPool();
		
		if (MyController)
		{
			MyController->OnBasicWarriorBeingDestroyed();
		}
	}
	else
	{
		this->Controller->Destroy();
		this->Destroy();
	}
}

float ABasicWarrior::GetValue()
{
	return Value;
}

void ABasicWarrior::SetIsDead(bool NewIsDeadState)
{
	AnimationData.IsDead = NewIsDeadState;
}

bool ABasicWarrior::GetIsDead()
{
	return AnimationData.IsDead;
}

void ABasicWarrior::OnObjectExitsPool()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);

	ABasicWarriorController* MyController = Cast<ABasicWarriorController>(this->GetController());
	
	if (MyController)
	{
		MyController->ActivateBehaviour();
	}

	if(AudioComponent->Event.Get())
	{
		AudioComponent->Play();
	}

	ActivateDissolveAppear();

	PlayersHit[0] = nullptr;
	PlayersHit[1] = nullptr;
	SetActorTickEnabled(true);
	GetLifeComponent()->ResetHitPoints();
	AnimationData.HasTakenDamage = AnimationData.IsDead = AnimationData.IsDisconnected = AnimationData.IsUsingTheirWeapon = false;
}

void ABasicWarrior::OnObjectEnterPool()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	
	ABasicWarriorController* MyController = Cast<ABasicWarriorController>(this->GetController());

	if (MyController)
	{
		MyController->StopExecutingAI();
	}

	PlayersHit[0] = nullptr;
	PlayersHit[1] = nullptr;
	SetActorTickEnabled(false);
	AnimationData.HasTakenDamage = AnimationData.IsDead = AnimationData.IsDisconnected = AnimationData.IsUsingTheirWeapon = false;
	SetActorLocation(OwnerPool->GetActorLocation());
}

void ABasicWarrior::OnReceiveDamageEnd()
{
	AnimationData.HasTakenDamage = false;
}

void ABasicWarrior::OnLogicDeath()
{
	//Disable damage and enable death
	AnimationData.HasTakenDamage = false;
	AnimationData.IsDead = true;

	//disable collision
	SetActorEnableCollision(false);

	//disable AI
	ABasicWarriorController* MyController = Cast<ABasicWarriorController>(this->GetController());

	if (MyController)
	{
		MyController->OnBasicWarriorBeingDestroyed();
	}

	CoinLootComponent->SpawnCoins();
}

void ABasicWarrior::OnDeadAnimationEnd()
{
	if (OwnerPool)
	{
		ReturnToPool();
	}
	else
		Destroy();
}

void ABasicWarrior::Disconnect()
{
	//Set is disconnected
	AnimationData.IsDisconnected = true;

	//disable AI
	ABasicWarriorController* MyController = Cast<ABasicWarriorController>(this->GetController());

	if (MyController)
	{
		MyController->OnBasicWarriorBeingDestroyed();
	}
}

void ABasicWarrior::ActivateDissolveDisappear()
{
	const FName ParamName("Dissolve");
	SetActorTickEnabled(true);
	DissolveCurrentTime = 0;
	BodyDissolveMaterial->SetScalarParameterValue(ParamName, 0);
	BladesDissolveMaterial->SetScalarParameterValue(ParamName, 0);
	DissolveAppear = false;
}

void ABasicWarrior::ActivateDissolveAppear()
{
	const FName ParamName("Dissolve");
	SetActorTickEnabled(true);
	DissolveCurrentTime = 0;
	BodyDissolveMaterial->SetScalarParameterValue(ParamName, 1);
	BladesDissolveMaterial->SetScalarParameterValue(ParamName, 1);
	DissolveAppear = true;
}

const struct FBasicWarriorAnimationData & ABasicWarrior::GetAnimationData()
{
	return AnimationData;
}
