// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TargetTauntActor.h"
#include "Structure/TwalienAnimationData.h"
#include "Twalien.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerHasDied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerExchangesWeapons);

UCLASS()
class TWALIENS_API ATwalien : public ACharacter
{
	GENERATED_BODY()

private:
	//float RespawnCountdown;

	//Validate if the player can respawn
	bool IsSpawnable;
	//FTimerHandle WeaponSkillHandler;
	FTimerHandle TauntSkillHandler;
	float CurrentHealth;
	class UForceFeedbackEffect* Vibration;
	bool IsDustpanPlayer;

	int ControllerID;
	APlayerController* MyController;

	FRotator DefaultRotation;

	//Structure that stores all data related to animation system
	FTwalienAnimationData AnimationData;

	//Time that player must hold attack button to consider that is held
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings", meta = (AllowPrivateAccess = "true"))
	float HoldWeaponTime;

	//Current hold time
	float CurrentHoldTime;

	//Delegate for HP component Death Event
	FScriptDelegate OnDeathDelegate;

	//Are player switching their weapons
	bool SwitchingWeapons;

	//Timer for when player is doing nothing change idle animation to second
	FTimerHandle ChangeIdleAnimationTimer;

	//Number of seconds for change idle animation
	float SecondsForChangeIdleAnimation;

	//Deviation of time for idle animation change
	float IdleAnimationChangeDeviation;

	//Delegate that send messages to observers when player is dead
	FPlayerHasDied PlayerDeadDelegate;

	//Delegate that send messages to observers when player weapon exchange is did it
	FPlayerExchangesWeapons PlayerExchangeDelegate;

	class APlayerCameraManager* camManager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Called when game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	/*************************************************************/
	//						DATA MEMBERS
	/*************************************************************/
	
	//Rotation Delta
	UPROPERTY(EditAnywhere, Category = "Player Settings")
	float DeltaYawRotation;
	
	//Bool for the players to use skills
	UPROPERTY(EditAnywhere, Category = "Player Settings")
	bool CanUseSkills;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Player Settings")
	class USharedLifeComponent* HPComponent;

	//Player's weapon skeletor
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Player Settings")
	class USkeletalMeshComponent* WeaponSkeletor;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Player Settings")
	class UWeapon* PlayerWeapon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Player Settings")
	class UDustpan* PlayerDustpan;

	//Player's weapon
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Player Settings")
	class USphereComponent* Sphere;

	//Validate if payer has died
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills Settings")
	float TauntSkillCD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills Settings")
	float TauntSkillInternalCD;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Player Settings")
	class UAimingAssistComponent* AimingAssist;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Player Settings")
	class UWidgetComponent * WidgetCompass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
		class UParticleSystemComponent* DamageEffectComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
		class UFMODAudioComponent* HittedAudioComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
		class UParticleSystemComponent* SwitchEffectComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
		class UParticleSystemComponent* RespawnerEffectComp = nullptr;


	bool IsAttacking;

	/*************************************************************/
	//					  FUNCTION MEMBERS
	/*************************************************************/

	//Called every frame for forward movement
	UFUNCTION(BlueprintCallable)
	void MoveForward(float value);

	UFUNCTION(BlueprintCallable)
	void LookUp(float value);

	//Called for side to side input
	UFUNCTION()
	void MoveRight(float Value);

	//Called every frame for rotation
	UFUNCTION()
	void LookRight(float value);

	//Called every frame for rotation
	UFUNCTION()
	void Aim();

	UFUNCTION()
	void Fire(float value);

	UFUNCTION()
	void AltFire(float value);

	UFUNCTION()
	void WeaponSkill();

	UFUNCTION()
	void ShowHabilitiesMenu();

	UFUNCTION()
	void HideHabilitiesMenu();

	UFUNCTION()
	void GetNextHability();

	UFUNCTION()
	void GetPreviousHability();

	UFUNCTION()
	void TauntSkill();

	UPROPERTY(EditDefaultsOnly, Category = "TauntDummy")
	TSubclassOf<ATargetTauntActor> TauntTargetClass;

	//Function that is called when players life come to 0
	//this is not the real death of player
	//this only fires death animation and do some stuff
	UFUNCTION()
	void OnLogicDeath();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	FVector DustpanRelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	FRotator DustpanRelativeRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	FVector GunRelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	FRotator GunRelativeRotation;

public:	

	// Sets default values for this character's properties
	ATwalien();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SwitchToWeapon(class UWeapon* OtherPlayerWeapon, class USkeletalMesh& WeaponMesh, class UMaterialInstanceDynamic & WeaponMaterial);

	void SwitchToDustpan(class UDustpan* OtherPlayerDustpan, class USkeletalMesh& DustpanMesh, class UMaterialInstanceDynamic & DustpanMaterial);

	void SetCurrentHealth(float HitPoints);

	UFUNCTION(BlueprintCallable)
	void ClearTauntSkillCooldown();

	UFUNCTION()
	class USkeletalMeshComponent* GetWeaponSkeletor() const;

	UFUNCTION()
	class USphereComponent* SpawnProjectile();

	UFUNCTION(BlueprintCallable)
	bool GetIsAttacking() const;
	
	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool AttackState);
	
	UFUNCTION(BlueprintCallable)
	class USharedLifeComponent* GetLifeComponent() const;

	UFUNCTION(BlueprintCallable)
	class UWeapon* GetPlayerWeapon() const;

	UFUNCTION(BlueprintCallable)
	bool GetIsDustpanPlayer() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilities")
	bool IsShowingHabilitiesMenu;

	UFUNCTION(BlueprintCallable)
	int GetCurrentHability();

	UPROPERTY(VisibleAnywhere, category = "Limits Habilities")
	int CurrentHability;

	UFUNCTION()
	void ActivateVibration(AController* Value);

	UFUNCTION()
	void SetControllerID(int ID);

	UFUNCTION(BlueprintCallable)
	const FTwalienAnimationData & GetAnimationData() const;

	//Set Animation field for playing normal idle animation
	UFUNCTION(BlueprintCallable)
	void SetNormalIdleAnimation();

	//Set Animation field for playing alternative animation
	UFUNCTION(BlueprintCallable)
	void SetAlternativeIdleAnimation();

	//Set Animation field for finish Hit animation
	UFUNCTION(BlueprintCallable)
	void OnReceiveDamageEnd();

	//Do Real player Death stuff
	UFUNCTION(BlueprintCallable)
	void OnAnimationDeath();

	//Do Spawn stuff
	UFUNCTION(BlueprintCallable)
	void OnPlayerSpawn();

	//Enables this player
	void Enable();

	//Disables player this player
	void Disable();

	//Death Delegate Access functions
	void AddDelegateToPlayerDeath(FScriptDelegate & Delegate);
	void AddDelegateToPlayerDeath(FScriptDelegate && Delegate);
	void RemoveDelegateFromPlayerDeath(const FScriptDelegate & Delegate);
	void ClearAllDelegatesFromPlayerDeath();

	//Player Event for when Weapon Switching is starting
	void OnWeaponSwitchingStart();

	//Player Event for when players throws their weapons for switch
	UFUNCTION(BlueprintCallable)
	void OnWeaponSwitchThrowWeapon();

	//Player Event for when players throws their weapons for switch
	UFUNCTION(BlueprintCallable)
	void OnWeaponSwitchingEnd();

	//Exchange Delegate Access functions
	void AddDelegateToPlayerExchange(FScriptDelegate & Delegate);
	void AddDelegateToPlayerExchange(FScriptDelegate && Delegate);
	void RemoveDelegateFromPlayerExchange(const FScriptDelegate & Delegate);
	void ClearAllDelegatesFromPlayerExchange();

	void SetBodyHeadMaterial(class UMaterialInterface & BodyMaterial, class UMaterialInterface & HeadMaterial, int DepthStencilValue);
	
	FORCEINLINE class UDustpan* GetPlayerDustpan() const { return PlayerDustpan; }
	FORCEINLINE int GetControllerID() const { return ControllerID; }
	FORCEINLINE UFMODAudioComponent* GetHittedAudioComponent() const { return HittedAudioComponent; }
	FORCEINLINE class APlayerController * GetPlayerController() const { return MyController; }
	FORCEINLINE void SetPlayerController(class APlayerController & NewPlayerController) { MyController = &NewPlayerController; }
	FORCEINLINE bool GetIsSpawnable() const { return IsSpawnable; }
	FORCEINLINE void SetIsSpawnable(bool NewSpawnableValue) { IsSpawnable = NewSpawnableValue; }
	FORCEINLINE bool GetIsDead() const { return IsDead; }
	FORCEINLINE void SetIsDead(bool Dead) { IsDead = Dead; }
};
