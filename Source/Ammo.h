// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SuckableObjects.h"
#include "Interfaces/IPooledObject.h"
#include "Ammo.generated.h"

UCLASS()
class TWALIENS_API AAmmo : public AActor, public ISuckableObjects, public IIPooledObject
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component Settings")
	class UStaticMeshComponent* AmmoMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component Settings")
	class UBoxComponent * AmmoCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin Settings")
	float Value;

public:
	// Sets default values for this actor's properties
	AAmmo();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnBeingSucked(const class UDustpan & Dustpan) override;

	virtual void OnObjectExitsPool() override;

	virtual void OnObjectEnterPool() override;

	FORCEINLINE float GetValue() const { return Value; }
	FORCEINLINE void SetValue(float NewValue) { Value = NewValue; }
	FORCEINLINE class UBoxComponent * GetCollider() { return AmmoCollider; }
};
