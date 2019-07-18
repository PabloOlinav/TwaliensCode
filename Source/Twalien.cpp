// All rights reserved.

#include "Twalien.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Engine/World.h"
#include "Weapon.h"
#include "SharedLifeComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "CameraLimitsComponent.h"
#include "ParticlesSceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "ConstructorHelpers.h"
#include "TargetTauntActor.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Dustpan.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "AimingAssistComponent.h"
#include "WidgetComponent.h"
#include "UserWidget.h"
#include "Structure/TwalienAnimationData.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Particles/ParticleSystemComponent.h"


class ATargerTauntActor;
// Sets default values
ATwalien::ATwalien() : Super(), DeltaYawRotation(180.f), IsDead(false), IsSpawnable(false), IsDustpanPlayer(false), CurrentHability(0), TauntSkillCD(60), TauntSkillInternalCD(0), CanUseSkills(false), HoldWeaponTime(0.15f), CurrentHoldTime(0), SwitchingWeapons(false), SecondsForChangeIdleAnimation(5.f), IdleAnimationChangeDeviation(1.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//FMod initialization
	IFMODStudioModule::Get();

	//Weapon Skeletor
	//ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMesh(TEXT("/Game/Materials/Mesh/SK_FPGun"));
	ConstructorHelpers::FObjectFinder<UForceFeedbackEffect> VibrationEffect(TEXT("/Game/Gameplay/Gamepad/Vibrate"));
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBpClass(TEXT("/Game/Animations/CharacterTwalien/Twalien_01_AnimBP"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshSkeletalMesh(TEXT("/Game/Characters/CharacterTwalien/SK_CharacterTwalien_01"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshFirstMaterial(TEXT("/Game/Characters/Materials/M_Character_01"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshSecondMaterial(TEXT("/Game/Characters/Materials/M_CharacterFace_01"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> CompassMaterial(TEXT("/Engine/EngineMaterials/Widget3DPassThrough_Masked_OneSided.Widget3DPassThrough_Masked_OneSided"));
	ConstructorHelpers::FClassFinder<UUserWidget> CompassWidgetClass(TEXT("/Game/Interface/UI_Compass"));

	
	WeaponSkeletor = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TwalienWeaponSkeletor"));
	
	//WeaponSkeletor->SetRelativeLocation(FVector(0.0f, 6.0f, 0.0f));
	//WeaponSkeletor->SetRelativeRotation(FRotator(0, 90.0f, 6));

	//if(WeaponMesh.Succeeded())
	//{
	//	WeaponSkeletor->SetSkeletalMesh(WeaponMesh.Object);
	//}

	WeaponSkeletor->SetupAttachment(GetMesh(), FName("Bip001-R-HandSocket"));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("TwalienSphere"));
	Sphere->SetupAttachment(WeaponSkeletor);
	Sphere->SetSphereRadius(3.0f);
	Sphere->SetRelativeLocation(FVector(0.0f, 55.0f, 8.0f));
	Sphere->SetRelativeRotation(FRotator(0, 90.0f, 0));

	DefaultRotation = FRotator(0, 90.0f, 0);

	//Weapon
	PlayerWeapon = CreateDefaultSubobject<UWeapon>(TEXT("TwalienWeaponComponent"));
	PlayerWeapon->SetupAttachment(Sphere);

	//Dustpan
	PlayerDustpan = CreateDefaultSubobject<UDustpan>(TEXT("TwalienDustpanComponent"));
	PlayerDustpan->SetupAttachment(Sphere);

	//Life component
	HPComponent = CreateDefaultSubobject<USharedLifeComponent>(TEXT("SharedLifeComponent"));
	OnDeathDelegate.BindUFunction(this, FName("OnLogicDeath"));

	HPComponent->AddDelegateToDeath(OnDeathDelegate);
	AddOwnedComponent(HPComponent);

	//CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 560.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->MaxWalkSpeed = 550.f;

	//Mesh
	//USkeletalMeshComponent* OwnMesh;
	//
	//OwnMesh = GetMesh();
	
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//OwnMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	if(AnimBpClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBpClass.Class);
	}

	if(MeshSkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshSkeletalMesh.Object);
	}

	if(MeshFirstMaterial.Succeeded() && MeshSecondMaterial.Succeeded())
	{
		GetMesh()->SetMaterial(0, MeshFirstMaterial.Object);
		GetMesh()->SetMaterial(1, MeshSecondMaterial.Object);
	}

	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	GetMesh()->bRenderCustomDepth = true;
	GetMesh()->CustomDepthStencilValue = 2;

	if (CanUseSkills) {

		ConstructorHelpers::FClassFinder<ATargetTauntActor> TargetTaunt(TEXT("/Game/Blueprint/BP_TargetTauntActor"));

		TauntTargetClass = TargetTaunt.Class;
	}

	AimingAssist = CreateDefaultSubobject<UAimingAssistComponent>(FName("AimingAssist"));
	AimingAssist->SetupAttachment(RootComponent);

	AimingAssist->SetRelativeLocation(FVector(610.0f, 0.0f, 50.0f));
	AimingAssist->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	AimingAssist->SetRelativeScale3D(FVector(1.0f, 6.0f, 12.0f));

	WidgetCompass = CreateDefaultSubobject<UWidgetComponent>(TEXT("TwalienCompass"));
	WidgetCompass->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	WidgetCompass->SetRelativeRotation(FRotator(90.f, 0.f, 180.f).Quaternion());

	if (CompassMaterial.Succeeded())
	{
		WidgetCompass->SetMaterial(0, CompassMaterial.Object);
	}

	if (CompassWidgetClass.Succeeded())
	{
		WidgetCompass->SetWidgetClass(CompassWidgetClass.Class);
	}

	WidgetCompass->SetPivot(FVector2D(0.35f, 0.33f));
	WidgetCompass->SetCollisionProfileName(FName("NoCollision"));
	WidgetCompass->SetupAttachment(RootComponent);

	HittedAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Hitted Sound"));
	HittedAudioComponent->SetupAttachment(GetMesh());

	Vibration = VibrationEffect.Object;

	//Initialize Animation data Packet
	AnimationData.ForwardVector = 0.f;
	AnimationData.RightVector = 0.f;
	AnimationData.HasTakenDamage = false;
	AnimationData.BoredForWaitingInput = 0;
	AnimationData.IsDeath = false;
	AnimationData.IsDustpanPlayer = false;
	AnimationData.IsHoldingShoot = false;
	AnimationData.IsUsingTheirWeapon = false;
	AnimationData.IsSwitchingWeapons = false;

	DustpanRelativeLocation = FVector(-3.0f, -15.0f, -3.227285f);
	DustpanRelativeRotation = FRotator(-8.01945f, -33.239544f, -73.481956f);

	// Particles
	DamageEffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Damage Effect"));
	DamageEffectComp->SetupAttachment(RootComponent);

	SwitchEffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Switch Particles Effect"));
	SwitchEffectComp->SetupAttachment(RootComponent);

	RespawnerEffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Respawner Particles Effect"));
	RespawnerEffectComp->SetupAttachment(RootComponent);
	
	ConstructorHelpers::FObjectFinder<UParticleSystem> DmgFX(TEXT("/Game/Effects/Particles/PS_PJDamage_01"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> SwitchFX(TEXT("/Game/Effects/Particles/PS_SwitchEffect_01"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> RespawnFX(TEXT("/Game/Effects/Particles/PS_PJRespawn_01"));

	if (SwitchFX.Succeeded())
	{
		SwitchEffectComp->SetTemplate(SwitchFX.Object);
	}

	if (DmgFX.Succeeded())
	{
		DamageEffectComp->SetTemplate(DmgFX.Object);
	}

	if (RespawnFX.Succeeded())
	{
		RespawnerEffectComp->SetTemplate(RespawnFX.Object);
	}
	
}

void ATwalien::OnLogicDeath()
{
	AnimationData.HasTakenDamage = false;
	AnimationData.IsDeath = true;
	DisableInput(MyController);
}

// Called when the game starts or when spawned
void ATwalien::BeginPlay()
{
	Super::BeginPlay();
	DamageEffectComp->SetWorldScale3D(FVector(2.0f));

}

// Called every frame
void ATwalien::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Aim();

	ActivateVibration(MyController);

	const UWorld * World = GetWorld();
	//Check if player is moving
	if (FMath::IsNearlyZero(AnimationData.ForwardVector) && FMath::IsNearlyZero(AnimationData.RightVector))
	{
		//Is idle change timer is inactive
		if (!World->GetTimerManager().IsTimerActive(ChangeIdleAnimationTimer))
		{
			//Create a random delay time between [time - deviation, time + deviation]
			float Delay = FMath::RandRange(SecondsForChangeIdleAnimation - IdleAnimationChangeDeviation, SecondsForChangeIdleAnimation + IdleAnimationChangeDeviation);

			//Activate timer
			World->GetTimerManager().SetTimer(ChangeIdleAnimationTimer, this, &ATwalien::SetAlternativeIdleAnimation, 1., false, Delay);
		}
	}
	else
	{
		//Check timer is active
		if (World->GetTimerManager().IsTimerActive(ChangeIdleAnimationTimer))
		{
			//Clear time because we are moving
			World->GetTimerManager().ClearTimer(ChangeIdleAnimationTimer);
		}
		//set animation field to false, this is because we can be moving and when we stop we dont want to play idle 2 animation
		AnimationData.BoredForWaitingInput = false;
	}

	if(HPComponent->GetHitPoints() <= 0.0f)
	{
		IsDead = true;
	}
}

void ATwalien::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	HPComponent->UnregisterComponent();
	HPComponent->DestroyComponent();
}

// Called to bind functionality to input
void ATwalien::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Fire", this, &ATwalien::Fire);
	PlayerInputComponent->BindAxis("AltFire", this, &ATwalien::AltFire);
	PlayerInputComponent->BindAxis("Forward", this, &ATwalien::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &ATwalien::MoveRight);
	PlayerInputComponent->BindAxis("LookRight");
	PlayerInputComponent->BindAxis("LookUp");
	
	// Binding key for habilities menu
	if (CanUseSkills) 
	{
		PlayerInputComponent->BindAction("HabilitiesMenu", IE_Pressed, this, &ATwalien::ShowHabilitiesMenu);
		PlayerInputComponent->BindAction("HabilitiesMenu", IE_Released, this, &ATwalien::HideHabilitiesMenu);
		PlayerInputComponent->BindAction("MoveToNextHability", IE_Released, this, &ATwalien::GetNextHability);
		PlayerInputComponent->BindAction("PreviousHability", IE_Released, this, &ATwalien::GetPreviousHability);
		PlayerInputComponent->BindAction("WeaponSkill", IE_Pressed, this, &ATwalien::WeaponSkill);
		PlayerInputComponent->BindAction("TauntSkill", IE_Pressed, this, &ATwalien::TauntSkill);
	}
}

//Called every frame for forward movement
void ATwalien::MoveForward(float value)
{	
	if (camManager)
	{
		AddMovementInput(camManager->GetActorForwardVector(), value);
	}
	else
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void ATwalien::MoveRight(float Value)
{
	if (camManager)
	{
		AddMovementInput(camManager->GetActorRightVector(), Value);
	}
	else
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

//Called every frame for rotation
void ATwalien::LookRight(float value)
{
	if(!FMath::IsNearlyZero(value))
	{
		//AddControllerYawInput(value * DeltaYawRotation * GetWorld()->GetDeltaSeconds());
		//FRotator Rot = FRotator(0.0f, GetActorRotation().Yaw + DeltaYawRotation * value * GetWorld()->GetDeltaSeconds(), 0.0f);
		//SetActorRotation(Rot);
	}
}

void ATwalien::Aim()
{
	FVector Direction(this->InputComponent->GetAxisValue("LookUp"), this->InputComponent->GetAxisValue("LookRight"), 0.0f);

	FRotator Rot = Direction.Rotation();

	//Take New Reference System and apply to Movement Vector
	FVector TransformedVector = Rot.RotateVector(FVector(this->InputComponent->GetAxisValue("Right"), this->InputComponent->GetAxisValue("Forward"), 0));
	//We overwrite animation data fields with transformed vector
	AnimationData.ForwardVector = TransformedVector.Y;
	AnimationData.RightVector = TransformedVector.X;

	if (camManager)
	{
		Rot.Yaw += camManager->GetCameraRotation().Yaw;
	}

	if (Direction.Size() > 0.25f)
	{
		this->GetController()->SetControlRotation(Rot);
	}
}

//Called every frame for rotation
void ATwalien::LookUp(float value)
{
	//AddMovementInput(GetActorForwardVector(), value);
	if (!FMath::IsNearlyZero(value))
	{
		//AddControllerPitchInput(value);
		//AddControllerYawInput(value * DeltaYawRotation * GetWorld()->GetDeltaSeconds());
		//FRotator Rot = FRotator(0.0f, GetActorRotation().Yaw + DeltaYawRotation * value * GetWorld()->GetDeltaSeconds(), 0.0f);
		//SetActorRotation(Rot);
	}
}

//Called every frame for weapon action check
void ATwalien::Fire(float value)
{
	//If button is almost pressed
	if (value > 0.8 && !AnimationData.IsSwitchingWeapons)
	{
		//We add delta time to current hold time
		CurrentHoldTime += GetWorld()->GetDeltaSeconds();

		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;

		bUseControllerRotationYaw = true;

		if (IsDustpanPlayer)
		{
			PlayerDustpan->Suck();
			PlayerDustpan->ActivateParticules(true);
			
		}
		else
		{
			FRotator AimDirection;

			AimDirection = AimingAssist->AimToTarget(Sphere->GetComponentLocation());

			if(!(AimDirection == FRotator::ZeroRotator))
			{
				Sphere->SetWorldRotation(AimDirection, true);
			}
			else
			{
				Sphere->SetRelativeRotation(DefaultRotation);
			}
			
			PlayerWeapon->Fire();
		}

		IsAttacking = true;
	}
	else
	{
		//Reset Hold Time
		CurrentHoldTime = 0;

		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		IsAttacking = false;

		if (IsDustpanPlayer)
		{
			PlayerDustpan->GetSuckAudioComponent()->Stop();
			GetWorldTimerManager().ClearTimer(PlayerDustpan->GetPlayEffectHandle());
			PlayerDustpan->ActivateParticules(false);
		}
	}

	//We set Attack animation data value to IsAttacking value
	AnimationData.IsUsingTheirWeapon = IsAttacking;
	
	//We set hold time to current hold time
	AnimationData.IsHoldingShoot = CurrentHoldTime >= HoldWeaponTime;
}

void ATwalien::AltFire(float value)
{
	//If button is almost pressed
	if (value > 0.8 && !AnimationData.IsSwitchingWeapons)
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;

		//if (WeaponType->GetName() == "ShootingGunComponent")
		//{
			bUseControllerRotationYaw = true;
		//}

		//AltFireDelegate.ExecuteIfBound();
		if (!IsDustpanPlayer)
		{
			PlayerWeapon->AltFire();
		}
		IsAttacking = true;
	}
	else
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		IsAttacking = false;
	}
}

void ATwalien::ShowHabilitiesMenu()
{
	IsShowingHabilitiesMenu = true;
}

void ATwalien::HideHabilitiesMenu()
{
	IsShowingHabilitiesMenu = false;
}

void ATwalien::GetNextHability()
{
	if(IsShowingHabilitiesMenu)
	{
		if (CurrentHability <= 1)
			CurrentHability++;

		else if (CurrentHability >= 2)
			CurrentHability = 0;
	}
}

void ATwalien::GetPreviousHability()
{
	if(IsShowingHabilitiesMenu)
	{
		if (CurrentHability > 0)
			CurrentHability--;

		else if (CurrentHability <= 0)
			CurrentHability = 2;
	}
}

void ATwalien::SwitchToWeapon(UWeapon* OtherPlayerWeapon, USkeletalMesh& WeaponMesh, UMaterialInstanceDynamic & WeaponMaterial)
{
	DamageEffectComp->DeactivateSystem();
	IsDustpanPlayer = false;

	//We set if this player has Dustpan in animation data package
	AnimationData.IsDustpanPlayer = IsDustpanPlayer;
	
	WeaponSkeletor->SetSkeletalMesh(&WeaponMesh);
	WeaponSkeletor->SetMaterial(0, &WeaponMaterial);

	WeaponSkeletor->SetRelativeLocation(GunRelativeLocation);
	WeaponSkeletor->SetRelativeRotation(GunRelativeRotation);

	Sphere->SetRelativeLocation(FVector(0.0f, 55.0f, 8.0f));

	if (OtherPlayerWeapon)
	{
		PlayerWeapon->TransferState(*OtherPlayerWeapon);
	}

	SwitchEffectComp->ActivateSystem(true);
	
	//SwitchEffectComp->ActivateParticles();
}

void ATwalien::SwitchToDustpan(UDustpan	* OtherPlayerDustpan, USkeletalMesh& DustpanMesh, UMaterialInstanceDynamic & DustpanMaterial)
{
	DamageEffectComp->DeactivateSystem();

	IsDustpanPlayer = true;

	//We set if this player has Dustpan in animation data package
	AnimationData.IsDustpanPlayer = IsDustpanPlayer;

	WeaponSkeletor->SetSkeletalMesh(&DustpanMesh);
	WeaponSkeletor->SetMaterial(0, &DustpanMaterial);

	WeaponSkeletor->SetRelativeLocation(DustpanRelativeLocation);
	WeaponSkeletor->SetRelativeRotation(DustpanRelativeRotation);

	Sphere->SetRelativeLocation(FVector(0.0f, 40.0f, 0.0f));
	Sphere->SetRelativeRotation(FRotator(0, 90.0f, 0));

	if (OtherPlayerDustpan)
	{
		PlayerDustpan->TransferState(*OtherPlayerDustpan);
	}

	SwitchEffectComp->ActivateSystem(true);
	//SwitchEffectComp->ActivateParticles();
}

void ATwalien::SetCurrentHealth(float HitPoints)
{
	CurrentHealth = HitPoints;
}

USkeletalMeshComponent* ATwalien::GetWeaponSkeletor() const
{
	return WeaponSkeletor;
}

int ATwalien::GetCurrentHability()
{
	return CurrentHability;
}

void ATwalien::ActivateVibration(AController* Value)
{
	if (HPComponent->GetHitPoints() < CurrentHealth && HPComponent->GetHitPoints() >= 0)
	{
		if (DamageEffectComp->bWasDeactivated)
			DamageEffectComp->ActivateSystem(true);

		CurrentHealth = HPComponent->GetHitPoints();
		//We set we receive damage field on animation data package
		AnimationData.HasTakenDamage = true;

		if (Value)
		{
			Cast<APlayerController>(Value)->ClientPlayForceFeedback(Vibration, false, true, FName("Damaged Feedback"));
		}

	}
	

}

void ATwalien::SetControllerID(int ID)
{
	ControllerID = ID;
	MyController = UGameplayStatics::GetPlayerController(GetWorld(), ControllerID);
	camManager = MyController->PlayerCameraManager;
}

void ATwalien::SetNormalIdleAnimation()
{
	AnimationData.BoredForWaitingInput = false;
}

void ATwalien::SetAlternativeIdleAnimation()
{
	AnimationData.BoredForWaitingInput = true;
}

void ATwalien::OnReceiveDamageEnd()
{
	AnimationData.HasTakenDamage = false;
}

void ATwalien::OnAnimationDeath()
{
	PlayerDeadDelegate.Broadcast();
	AnimationData.IsDeath = false;
	AnimationData.HasTakenDamage = false;
}

void ATwalien::OnPlayerSpawn()
{
	Enable();
	RespawnerEffectComp->ActivateSystem(true);
}

void ATwalien::Enable()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	EnableInput(MyController);
	GetLifeComponent()->ResetHitPoints();
	SetCurrentHealth(HPComponent->GetHitPoints());
	SetIsDead(false);
	AnimationData.IsDeath = IsDead;
}

void ATwalien::Disable()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	DisableInput(MyController);
	SetIsSpawnable(true);
	SetIsDead(true);
}

void ATwalien::AddDelegateToPlayerDeath(FScriptDelegate & Delegate)
{
	PlayerDeadDelegate.Add(Delegate);
}

void ATwalien::AddDelegateToPlayerDeath(FScriptDelegate && Delegate)
{
	PlayerDeadDelegate.Add(Delegate);
}

void ATwalien::RemoveDelegateFromPlayerDeath(const FScriptDelegate & Delegate)
{
	PlayerDeadDelegate.Remove(Delegate);
}

void ATwalien::ClearAllDelegatesFromPlayerDeath()
{
	PlayerDeadDelegate.Clear();
}

void ATwalien::OnWeaponSwitchingStart()
{
	//we dont want players move when they are switching weapons
	//DisableInput(MyController);
	//Set Animation data field to true to start switch animation
	AnimationData.IsSwitchingWeapons = true;
	//Set Animation data field of Hit to false
	AnimationData.HasTakenDamage = false;
}

void ATwalien::OnWeaponSwitchThrowWeapon()
{
	OnWeaponSwitchingEnd();
}

void ATwalien::OnWeaponSwitchingEnd()
{
	PlayerExchangeDelegate.Broadcast();
	EnableInput(MyController);
	AnimationData.HasTakenDamage = false;
	AnimationData.IsSwitchingWeapons = false;
}

void ATwalien::AddDelegateToPlayerExchange(FScriptDelegate & Delegate)
{
	PlayerExchangeDelegate.Add(Delegate);
}

void ATwalien::AddDelegateToPlayerExchange(FScriptDelegate && Delegate)
{
	PlayerExchangeDelegate.Add(Delegate);
}

void ATwalien::RemoveDelegateFromPlayerExchange(const FScriptDelegate & Delegate)
{
	PlayerExchangeDelegate.Remove(Delegate);
}

void ATwalien::ClearAllDelegatesFromPlayerExchange()
{
	PlayerExchangeDelegate.Clear();
}

void ATwalien::SetBodyHeadMaterial(class UMaterialInterface & BodyMaterial, class UMaterialInterface & HeadMaterial, int DepthStencilValue)
{
	USkeletalMeshComponent * MyMesh = GetMesh();
	if (MyMesh)
	{
		MyMesh->SetMaterial(0, &BodyMaterial);
		MyMesh->SetMaterial(1, &HeadMaterial);
		MyMesh->CustomDepthStencilValue = DepthStencilValue;
		WeaponSkeletor->CustomDepthStencilValue = DepthStencilValue;
	}
}

void ATwalien::WeaponSkill() 
{
	if (IsDustpanPlayer)
	{
		PlayerDustpan->DustpanSkillActivate();
	}
	else
	{
		PlayerWeapon->WeaponSkill();
	}
}

void ATwalien::TauntSkill()
{
	UWorld * World = GetWorld();
	if (World)
	{
		if (!World->GetTimerManager().IsTimerActive(TauntSkillHandler))
		{
			FActorSpawnParameters SpawnParams;
			ATargetTauntActor* TargetDummy = World->SpawnActor<ATargetTauntActor>(TauntTargetClass, GetActorLocation() + FVector(0, 0, 1) * 100, GetActorRotation(), SpawnParams);

			World->GetTimerManager().SetTimer(TauntSkillHandler, this, &ATwalien::ClearTauntSkillCooldown, 1.0f, false, TauntSkillCD);
		}
	}
}

USphereComponent* ATwalien::SpawnProjectile()
{
	return Sphere;
}

void ATwalien::ClearTauntSkillCooldown() 
{
	GetWorld()->GetTimerManager().ClearTimer(TauntSkillHandler);
}

bool ATwalien::GetIsAttacking() const
{
	return IsAttacking; 
}

void ATwalien::SetIsAttacking(bool AttackState)
{
	IsAttacking = AttackState; 
}

USharedLifeComponent * ATwalien::GetLifeComponent() const
{ 
	return HPComponent; 
}

UWeapon * ATwalien::GetPlayerWeapon() const
{
	return PlayerWeapon; 
}

bool ATwalien::GetIsDustpanPlayer() const
{
	return IsDustpanPlayer;
}

const FTwalienAnimationData & ATwalien::GetAnimationData() const
{
	return AnimationData;
}
