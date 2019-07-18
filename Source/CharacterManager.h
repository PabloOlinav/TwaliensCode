// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Twalien.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "CharacterManager.generated.h"

UCLASS()
class TWALIENS_API ACharacterManager : public AActor
{
	GENERATED_BODY()

private:
	TSubclassOf<ATwalien> Player;
	FTimerHandle SwitchTimerHandle;
	APlayerStart* AStartPoints;
	
	USkeletalMesh* GunMaterial;
	USkeletalMesh* DustpanMaterial;
	TSubclassOf<class UUserWidget> GameOverWidget;

	//Timer For Player Respawn
	FTimerHandle RespawnTimerHandle;

	//Function that be called when player death animation is over
	//Start respawn timer and does init respawn stuff
	UFUNCTION()
	void StartPlayerRespawnTimer();

	//Function for do player respawn stuff
	UFUNCTION()
	void RespawnPlayers();

	//ScriptDelegate for observe players death
	FScriptDelegate PlayerDeathDelegate;

	//Display Game Over Menu
	void ShowGameOverMenu();

	//ScriptDelegate for observe players Switch
	FScriptDelegate PlayerSwitchDelegate;

	//Flag to check if players is doing switch
	bool PlayerIsDoingSwitch;

	//Tells manager weapon switch is completed
	UFUNCTION()
	void SwitchCompleted();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	class UTwalienSkillComponent * TauntSkill;
	
	UPROPERTY(VisibleAnywhere, category = "Settings")
	TArray<AActor*> RespawnLocation;

	UPROPERTY(VisibleAnywhere, category = "Settings")
	TArray<ATwalien*> Players;

	UPROPERTY(EditAnywhere, category = "Settings")
	int Continues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool CreatePlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	class UFMODEvent* FireSoundEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	class UFMODEvent* SuckSoundEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	class UFMODAudioComponent* SwitchAudio;

	//Time Player have to wait to respawn when dead
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float RespawnTime;

	//Time to wait before the weapons switch
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float SwitchTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	class ULifeComponent* SourceTwaliensHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float GunCurseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool ShouldSwitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float SwitchDistance;

	class UUserWidget* GameOver;

	class UMaterialInterface * GunMaterialInterface, * DustpanMaterialInterface;
	class UMaterialInstanceDynamic * MIDGun, * MIDDustpan;
	class UMaterialInterface * Player1Body, * Player2Body, * Player1Head, * Player2Head;

	const FLinearColor Player1Color;
	const FLinearColor Player2Color;

public:	
	// Sets default values for this character's properties
	ACharacterManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Save the start points of the players
	void FindRespawnLocations();

	//Spawn the players
	void SpawnPlayers();

	UFUNCTION(BlueprintCallable)
	void FindPlayers();

	//Switch the weapon between players
	UFUNCTION(BlueprintCallable)
	void SwitchGun();

	//Disable the player
	UFUNCTION()
	void DisablePlayer(ATwalien* APlayer);

	//Enable the player
	void EnablePlayer(ATwalien* APlayer);

	UFUNCTION(BlueprintCallable)
	void GiveSomeAmmo(int AmmoAmount);

	UFUNCTION(BlueprintCallable)
	void GiveSomeAlternativeAmmo(int StingAmount);

	UFUNCTION(BlueprintCallable)
	class ATwalien * GetPlayerWithWeapon() const;

	UFUNCTION(BlueprintCallable)
	class ATwalien * GetPlayerWithDustpan() const;

	//Get Remaining time to Switch Weapons
	UFUNCTION(BlueprintCallable) 
	float GetRemainingSwitchTime() const;
	
	UFUNCTION(BlueprintCallable) 
	ATwalien* GetPlayer(int Index) const;
	
	UFUNCTION(BlueprintCallable) 
	float GetRemainingRespawnTime() const;

	UFUNCTION(BlueprintCallable)
	FTimerHandle GetSwitchTimerHandle();

	UFUNCTION(BlueprintCallable)
	void SetSwitchTimerHandle(float Time);

	UFUNCTION(BlueprintCallable)
		void SetRespawnPosition(AActor* Actor, int Index);

	void SetWeaponSounds();
	
	FORCEINLINE float PlayerAreApproximatelyAtSwitchDistance() const {
		return FMath::Abs(Players[0]->GetActorLocation().X - Players[1]->GetActorLocation().X) <= SwitchDistance && FMath::Abs(Players[0]->GetActorLocation().Y - Players[1]->GetActorLocation().Y) <= SwitchDistance;
	}
	FORCEINLINE float GetPlayersDistance() const { return FVector::Distance(Players[0]->GetActorLocation(), Players[1]->GetActorLocation()); }
	FORCEINLINE void SetContinues(int Value) { if (Continues > 0) { Continues += Value; } }
	FORCEINLINE int GetContinues() const { return Continues; }
};
