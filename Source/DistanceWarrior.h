// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DistanceWarrior.generated.h"

UCLASS()
class TWALIENS_API ADistanceWarrior : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADistanceWarrior();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
		class ULifeComponent* HPComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
		class UCoinLoot* CoinLootComponent;

	//Enemy's weapon
	UPROPERTY(VisibleDefaultsOnly, Category = "Enemy Settings")
		class UShieldComponent* Shield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
		class UWeapon* ShootingGun;

	UPROPERTY(EditAnywhere, Category = "Enemy Settings")
		float damage = 20;

	UPROPERTY(EditAnywhere, Category = "Player Settings")
		class APoolManager* OwnerPool;

	UPROPERTY(EditAnywhere, Category = "Player Settings")
		bool IsDead;

	bool IsAttacking;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SetOwnerPool(class APoolManager* Pool);

	UFUNCTION(BlueprintCallable)
		bool GetIsAttacking();

	UFUNCTION(BlueprintCallable)
		void SetIsAttacking(bool NewAttackState);

	UFUNCTION(BlueprintCallable)
		void ActivateFiring();

	UFUNCTION(BlueprintCallable)
		void OnDeath();

	UFUNCTION(BlueprintCallable)
		void Fire();

	UFUNCTION(BlueprintCallable)
		void DisablePlayer();

	UFUNCTION(BlueprintCallable)
		void EnablePlayer();

	UFUNCTION(BlueprintCallable)
		void SetIsDead(bool b);
};
