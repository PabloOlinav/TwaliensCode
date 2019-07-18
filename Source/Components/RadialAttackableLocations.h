// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeakObjectPtr.h"
#include "Structure/RelativePositionId.h"
#include "RadialAttackableLocations.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWALIENS_API URadialAttackableLocations : public UActorComponent
{
	GENERATED_BODY()

private:
	struct FQuadrant
	{
		TArray<FVector> Position;
		TArray<bool> InUse;
	};

	struct FQuadrants
	{
		TArray<FQuadrant> Quadrant;
	};

	//We divide location into four quadrant for performance when some ai request a location
	TArray<FQuadrants> RelativeAttackableLocation;

	void InitData(float Radius, float MaxEnemyRadius, int RingIndex);

	//Create attackable Location with given data
	void CreateLocations(int PartitionsAmount, float AngleOffset, int RingIndex, float Radius);

	bool GetEmptyPosition(int QuadrantIndex, int RingIndex, FVector & Position, FRelativePositionId & RelativePositionAccessInfo);

	bool SearchForPoint(int QuadrantIndex, FVector & Position, FRelativePositionId & RelativePositionAccessInfo);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
	TArray<float> EnemiesColliderRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
	float FirstRingRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
	float RestRingRadiusIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
	int RingAmount;

public:	
	// Sets default values for this component's properties
	URadialAttackableLocations();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool GetFreeRelativePosition(const FVector & AgentPosition, FVector & RelativePosition, FRelativePositionId & RelativePositionAccessInfo);

	UFUNCTION(BlueprintCallable)
	void GetRelativePosition(const FRelativePositionId & AccessInfo, FVector & RelativePosition);

	UFUNCTION(BlueprintCallable)
	void FreeRelativePosition(const FRelativePositionId & AccessInfo);
};
