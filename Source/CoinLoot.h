// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoinLoot.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWALIENS_API UCoinLoot : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCoinLoot();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
	//TODO - Interfaz para loot - a$tuR
	TSubclassOf<class AAmmo> LootClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
	int AmountCoins = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
	FVector SpawnOffset = FVector(0, 0, 100);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
	float RelativeHeightForceVector = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
	float ForceToApply = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Settings")
	float Radius = 60000.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SpawnCoins() const;
};
