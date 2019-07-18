// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TwalienSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWALIENS_API UTwalienSkillComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTwalienSkillComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
		float TauntRadio = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
		class USphereComponent * TauntArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
		TArray<FName> AllowedTagsToInteract;

	TArray<AActor*> ActorsInArea;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool HasActorAnyValidTag(AActor * Actor);
	bool IsEnemy(AActor * Actor);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
		void OnObjectEnterToArea(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnObjectExitToArea(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void TauntSkill();
		
};
