// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeFunctionality.h"
#include "SharedLifeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWALIENS_API USharedLifeComponent : public UActorComponent, public ILifeFunctionality
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USharedLifeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/*************************************************************/
	//						DATA MEMBERS
	/*************************************************************/

	class ULifeComponent * SourceLife;

	//Delegate that broadcast whenever hit points changes.
	UPROPERTY(BlueprintAssignable, Category = "Component Settings")
	FActorLifeHasBeenUpdated OnLifeChanged;

	//Delegate that broadcast whenever actor dies.
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Component Settings")
	FActorHasDied OnActorDeath;

	FScriptDelegate SourceUpdateDelegate;
	FScriptDelegate SourceDeathDelegate;

	UFUNCTION()
	void OnSourceLifeChange();

	UFUNCTION()
	void OnSourceLifeIsZero();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*************************************************************/
	//					  FUNCTION MEMBERS
	/*************************************************************/

	UFUNCTION(BlueprintCallable, Category = "Component Functions")
	//Return Actor Hit Points.
	virtual float GetHitPoints() const override;

	UFUNCTION(BlueprintCallable, Category = "Component Functions")
	//Reset Actor Hit Points.
	virtual void ResetHitPoints() override;

	UFUNCTION(BlueprintCallable, Category = "Component Functions")
	//Modify Actor Hit Points. 
	//DeltaHitPoints -> Hit Points added to Actor Hit Points.
	virtual void SetHitPoints(float DeltaHitPoints) override;

	//Add a function to life changes delegate.
	virtual void AddDelegateToHPUpdate(const FScriptDelegate & Delegate) override;

	//Add a function to delegate that broadcast whenever actor dies.
	virtual void AddDelegateToDeath(const FScriptDelegate & Delegate) override;

	//Remove a function from life changes delegate.
	virtual void RemoveDelegateFromHPUpdate(const FScriptDelegate & Delegate) override;

	//Clear all functions from life changes delegate.
	virtual void RemoveAllDelegatesFromHPUpdate() override;

	//Remove a function from death delegate.
	virtual void RemoveDelegateFromDeath(const FScriptDelegate & Delegate) override;

	//Clear all functions from death delegate.
	virtual void RemoveAllDelegatesFromDeath() override;

	UFUNCTION(BlueprintCallable)
	void SetCommonLifeComponent(class ULifeComponent * Source);

	UFUNCTION(BlueprintCallable)
	class ULifeComponent * GetCommonLifeComponent();

	virtual float GetMaxHitPoints() const override;

};
