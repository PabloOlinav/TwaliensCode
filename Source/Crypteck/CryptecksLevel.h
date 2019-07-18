// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CryptecksLevel.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API ACryptecksLevel : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ACryptecksLevel();

	FORCEINLINE FVector GetLevelCenter() { return LevelCenter; }

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool QueryInnerFreeCell(FVector &Position);

	bool QueryOuterFreeCell(FVector &Position);

	void ReturnInnerCell(const FVector & Position);

private:

	class ACrypteck * Crypteck;
	class AFireBall * CryptecksFireBall;
	class AToxicBall * CryptecksToxicBall;
	class ASpawnBall * CryptecksSpawnBall;
	class ASmashBall * CryptecksSmashBall;

	void InitializeCells();

	void InitializeCrypteckReferences();

	TArray<bool> InnerCells, OuterCells;
	TArray<int> InnerFreeCells, OuterFreeCells;
	TArray<FVector> InnerCellsPositions, OuterCellsPositions;

	int CurrentOuterIndex;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	FVector LevelCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	int AmountToxicAreas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	int AmountFireAreas;


};
