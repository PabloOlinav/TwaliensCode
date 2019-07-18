// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "AimingAssistComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWALIENS_API UAimingAssistComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAimingAssistComponent();

protected:
	
	class UStaticMesh* ConeCollider;

	TArray<AActor*> Target;

	FScriptDelegate BeginOverlapDelegate;

	FScriptDelegate EndOverlapDelegate;
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void GetTargets(TArray<AActor*>& Targets);

	UFUNCTION(BlueprintCallable)
		AActor* GetTarget();

	UFUNCTION(BlueprintCallable)
		FRotator AimToTarget(FVector Start);
};	
