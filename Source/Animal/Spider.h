// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/EnvironmentalAnimal.h"
#include "Spider.generated.h"

UCLASS()
class TWALIENS_API ASpider : public ACharacter, public IEnvironmentalAnimal
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spider")
	int RegionIndex;

public:	
	// Sets default values for this pawn's properties
	ASpider();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE virtual int GetRegionIndex() const { return RegionIndex; }
	FORCEINLINE virtual void SetRegionIndex(int NewRegionIndex) { RegionIndex = NewRegionIndex; }
};
