// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetTauntActor.generated.h"

UCLASS()
class TWALIENS_API ATargetTauntActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATargetTauntActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool HasActorAnyValidTag(AActor * Actor);
	bool IsEnemy(AActor * Actor);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component Settings")
	class USphereComponent * TauntArea;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component Settings")
	class UStaticMeshComponent * Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component Settings")
	class UBoxComponent * MeshCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Taunt Settings")
	float TauntRadio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Taunt Settings")
	float TauntTimeEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
	TArray<FName> AllowedTagsToInteract;
	
	void DestroyTaunt();

private:

	TArray<AActor*> ActorsInArea;

	void UnTaunt();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnObjectExitToArea(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void NotifyActorBeginOverlap(AActor * OtherActor);
	
	UFUNCTION()
	void TauntSkill();
};

