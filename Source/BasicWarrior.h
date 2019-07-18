// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/SuckableObjects.h"
#include "Interfaces/IPooledObject.h"
#include "Structure/BasicWarriorAnimationData.h"
#include "BasicWarrior.generated.h"

UCLASS()
class TWALIENS_API ABasicWarrior : public ACharacter, public ISuckableObjects, public IIPooledObject
{
	GENERATED_BODY()

private:
	FVector DefaultMeshLocation;
	FRotator DefaultMeshRotator;

	void MakeDamageHandsDamage(class AActor * OtherActor);

	TArray<class AActor *> PlayersHit;

	void DoDissolveAppear(float DeltaSeconds);
	
	void DoDissolveDisappear(float DeltaSeconds);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	class ULifeComponent * HPComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	class UCoinLoot * CoinLootComponent;

	UPROPERTY(EditAnywhere, Category = "Enemy Settings")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Enemy Settings")
	float MeleeDistance;

	UPROPERTY(VisibleAnywhere, Category = "Enemy Settings")
	class UBoxComponent* RightPunchTrigger;

	UPROPERTY(VisibleAnywhere, Category = "Enemy Settings")
	class UBoxComponent* LeftPunchTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	class UFMODAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage PS")
	class UParticlesSceneComponent* DamageEffectComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UParticleSystemComponent* PS_FloatingComp = nullptr;

	FScriptDelegate PunchOverlapDelegate;

	FScriptDelegate DeadDelegate, DamageDelegate;

	UFUNCTION()
	void OnPunchBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//Data package for animation
	FBasicWarriorAnimationData AnimationData;

	//Function that is called when hit point in life component changes
	UFUNCTION()
	void OnHealthChanged();

	//Variable for check y we have less health
	float LastHP;

	//Time for dissolve effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	float DissolveTotalTime;

	float DissolveCurrentTime;

	class UMaterialInstanceDynamic * BodyDissolveMaterial, * BladesDissolveMaterial;

	bool DissolveAppear;

public:
	// Sets default values for this character's properties
	ABasicWarrior();

	virtual void OnBeingSucked(const class UDustpan & Dustpan) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool GetIsAttacking();

	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool NewAttackState);

	UFUNCTION(BlueprintCallable)
	float GetAttackDistance();

	UFUNCTION(BlueprintCallable)
	void ActivatePunchDamage();

	UFUNCTION(BlueprintCallable)
	void DeactivatePunchDamage();

	UFUNCTION(BlueprintCallable)
	void OnDeath();

	UFUNCTION(BlueprintCallable)
	float GetValue();

	UFUNCTION(BlueprintCallable)
	void SetIsDead(bool NewIsDeadState);

	UFUNCTION(BlueprintCallable)
	bool GetIsDead();

	virtual void OnObjectExitsPool() override;

	virtual void OnObjectEnterPool() override;

	UFUNCTION(BlueprintCallable)
	const FBasicWarriorAnimationData & GetAnimationData();

	UFUNCTION(BlueprintCallable)
	void OnReceiveDamageEnd();

	UFUNCTION(BlueprintCallable)
	void OnLogicDeath();

	UFUNCTION(BlueprintCallable)
	void OnDeadAnimationEnd();

	UFUNCTION(BlueprintCallable)
	void Disconnect();

	UFUNCTION(BlueprintCallable)
	void ActivateDissolveDisappear();

	UFUNCTION(BlueprintCallable)
	void ActivateDissolveAppear();

	FORCEINLINE class ULifeComponent * GetLifeComponent() { return HPComponent; }
};
