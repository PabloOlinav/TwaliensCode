// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

UCLASS()
class TWALIENS_API ACoin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component Settings")
	class UStaticMeshComponent* CoinMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component Settings")
	class UBoxComponent * CoinCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin Settings")
	float value = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetValue();

	void SetValue(float NewValue);

	class UPrimitiveComponent * GetCollider();

};
