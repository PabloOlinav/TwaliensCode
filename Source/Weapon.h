
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ProjectileA.h"
#include "FMODAudioComponent.h"
#include "FMODEvent.h"
#include "Weapon.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TWALIENS_API UWeapon : public USceneComponent
{
	GENERATED_BODY()

private:
	FTimerHandle FireRateHandler;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Projectile class to spawn.
	UPROPERTY(EditAnywhere, Category = "Projectile")
		class APoolManager* BulletPool;

	UPROPERTY(EditAnywhere, Category = "Projectile")
		TSubclassOf<AProjectileA> ProjectileClass;

	//Amunnition
	UPROPERTY(EditAnywhere, Category = "Projectile")
		int Ammunition;

	//Alternative Amunnition
	UPROPERTY(EditAnywhere, Category = "Projectile")
		int AltAmmunition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
		float BaseFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
		float SkillFireRate;

	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
		FName ProjectileTag;

		FTimerHandle SkillHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
		float SkillDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
		UFMODAudioComponent* FireAudioComponent;

public:
	// Sets default values for this component's properties
	UWeapon();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void AltFire();
	
	UFUNCTION()
	void WeaponSkill();
	
	UFUNCTION()
	void DeactivateSkill();

	UFUNCTION()
	void FindPool();
 
	UFUNCTION(BlueprintCallable)
	int GetAmmunition() const;

	void TransferState(const UWeapon & Other);

	FORCEINLINE void SetProjectileClass(TSubclassOf<AProjectileA> NewProjectileClass) { ProjectileClass = NewProjectileClass; }
	FORCEINLINE void SetAmmunition(int AmmunitionAmount) { Ammunition += AmmunitionAmount; }
	FORCEINLINE void SetAltAmmunition(int AltAmmunitionAmount) { AltAmmunition += AltAmmunitionAmount; }
	FORCEINLINE int GetAltAmmunition() const { return AltAmmunition; }
	FORCEINLINE float GetBaseFireRate() const { return BaseFireRate; }
	FORCEINLINE void SetBaseFireRate(float NewBaseFireRate) { BaseFireRate = NewBaseFireRate; }
	FORCEINLINE float GetSkillFireRate() const { return SkillFireRate; }
	FORCEINLINE void SetSkillFireRate(float NewSkillFireRate) { SkillFireRate = NewSkillFireRate; }
	FORCEINLINE void SetProjectileTag(FName TagName) { ProjectileTag = TagName; }
	FORCEINLINE void SetFMODEvent(UFMODEvent* Event) { FireAudioComponent->Event = Event; }
};

