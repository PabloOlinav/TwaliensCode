// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrypteckArea.generated.h"

UCLASS(Abstract)
class TWALIENS_API ACrypteckArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrypteckArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CrypteckArea")
	class USphereComponent * MeshTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CrypteckArea")
	class UStaticMeshComponent * Mesh;

	TArray<class ATwalien *> PlayerInArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CrypteckArea")
	float MaximumDPS;

	float DPS;

	UFUNCTION()
	void OnAreaEnter(class UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, class UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const struct FHitResult & SweepResult);
	UFUNCTION()
	void OnAreaExit(class UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, class UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

	void DoDamageToPlayer(float DeltaTime) const;

	virtual void OnSpecificConstruct(float Percentage) { unimplemented(); }

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnConstruct(float Percentage);
};
