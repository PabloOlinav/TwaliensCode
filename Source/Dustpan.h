// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "FMODEvent.h"
#include "FMODAudioComponent.h"
#include "Dustpan.generated.h"

class UParticleSystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorHasBeenSucked, AActor *, SuckedActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWALIENS_API UDustpan : public USceneComponent
{
	GENERATED_BODY()

private:
	FTimerHandle PlayEffectHandle;

public:	
	// Sets default values for this component's properties
	UDustpan();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
		class USphereComponent* SuckArea;

	TArray<class ISuckableObjects *> ActorsInArea;

	FScriptDelegate OverlapEnterDelegate, OverlapExitDelegate;

	FActorHasBeenSucked OnActorHasBeenSucked;

	FTimerHandle DustpanSkillHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
		float SuckVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
		float SuckedDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
		float SuckRadio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
		float DustpanSkillCD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
		class UFMODAudioComponent* SuckAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
		UParticleSystemComponent* PS_DustpanComponent;

	

	int EnemiesSucked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Settings")
		int EnemiesToSuck;

	bool CanSuckEnemy;

	UFUNCTION()
		void OnObjectEnterToArea(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnObjectExitToArea(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

	//TODO - clase cache por DIOSSSS - a$tuR
	class ACharacterManager * CharacterManager;

	//Suckable object that doesn't allow dustpan suck other objects
	class ISuckableObjects * Exclusive;

	//Suckable object index in ActorsInArea array, we store this index for not reallocating TArray
	int ExclusiveIndex;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Add a function to delegate that broadcast whenever actor dies.
	void AddDelegateToSuck(const FScriptDelegate & Delegate);

	//Remove a function from life changes delegate.
	void RemoveDelegateFromSuck(const FScriptDelegate & Delegate);

	//Clear all functions from life changes delegate.
	void RemoveAllDelegatesFromSuck();

	void ActivateParticules(bool activateParticules);

	UFUNCTION()
		void Suck();

	UFUNCTION()
		void DustpanSkillActivate();

	UFUNCTION()
		void DustpanSkillDeactivate();

	void TransferState(const UDustpan & OtherDustpan);

	FORCEINLINE float GetSuckedDistance() const { return SuckedDistance; }
	FORCEINLINE float GetSuckVelocity() const { return SuckVelocity; }
	//workaround for not searching this on all ammo ...
	//TODO - cache class ffs
	FORCEINLINE class ACharacterManager * GetPlayerManager() const { return CharacterManager; }
	FORCEINLINE void SetFMODEvent(UFMODEvent* Event) { SuckAudioComponent->Event = Event; }
	FORCEINLINE UFMODAudioComponent* GetSuckAudioComponent() { return SuckAudioComponent; }
	FORCEINLINE FTimerHandle& GetPlayEffectHandle() { return PlayEffectHandle; }

	
};
