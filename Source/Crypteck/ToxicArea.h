// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrypteckArea.h"
#include "Interfaces/SuckableObjects.h"
#include "ToxicArea.generated.h"

UCLASS()
class TWALIENS_API AToxicArea : public ACrypteckArea, public ISuckableObjects
{
	GENERATED_BODY()
	
private:
	bool Constructed;

	float ConstructedPercentage;

	class ACryptecksLevel * Level;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnSpecificConstruct(float Percentage) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToxicArea")
	//1 / Seconds to be drain
	float DrainTimeModifier;

public:
	// Sets default values for this actor's properties
	AToxicArea();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool Drain(float DeltaSeconds);

	FORCEINLINE bool IsConstructed() const { return Constructed; }

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnBeingSucked(const class UDustpan & Dustpan) override;

};
