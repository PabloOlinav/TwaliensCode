// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ControlZone.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFragmentAtHome);

UCLASS()
class TWALIENS_API AControlZone : public AActor
{
	GENERATED_BODY()

private:
	

protected:

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Control Zone Settings", meta = (AllowPrivateAccess = "true"))
	class USphereComponent * ControlArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Control Zone Settings", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent * ControlMesh;
	
	//Gameplay data
	UPROPERTY(BlueprintReadOnly, Category = "Control Zone Data")
	int TotalInFragmentAmount;

	UPROPERTY(EditAnywhere, category = "Relics Settings")
	TArray<class APileFragment*> RelicsBoundToAltar;

	UPROPERTY(EditAnywhere, category = "Relics Settings")
	TArray<class ATargetPoint*> RelicsLocations;

	UPROPERTY(EditAnywhere, category = "Spawners Asociated")
	TArray<class AEnemySpawner*> Spawners;

	UFUNCTION(BlueprintCallable)
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(BlueprintAssignable)
	FFragmentAtHome OnFragmentCome;

	// Functions
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ActivateSpawners();

	UFUNCTION(BlueprintCallable)
	void DeactivateSpawners();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PS_ControlZone")
	class UParticleSystemComponent * PS_ControlZone;

	UPROPERTY(EditAnywhere, Category = "PS_ControlZone")
	bool bCanglow = true;


public:	

	AControlZone();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddObserverToProgress(FScriptDelegate Delegate);

	void RemoveObserverFromProgress(FScriptDelegate Delegate);

	void ClearObservers();

	UFUNCTION(BlueprintCallable)
	int GetTotalFragmentAmount();

	UFUNCTION(BlueprintCallable)
	void BindFragmentToAltar(class APileFragment * Fragment);

	UFUNCTION(BlueprintCallable)
	void UnbindFragmentToAltar(class APileFragment * Fragment);
};
