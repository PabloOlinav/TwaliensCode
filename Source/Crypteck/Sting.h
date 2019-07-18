// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SuckableObjects.h"
#include "Sting.generated.h"

UCLASS()
class TWALIENS_API ASting : public AActor, public ISuckableObjects
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sting")
	class UStaticMeshComponent * Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sting")
	class UBoxComponent * Collider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crypteck")
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crypteck")
	float Rotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crypteck")
	float DamageToPlayer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crypteck")
	float DamageToBall;
	
	bool Pickable, Hittable;

	UFUNCTION()
	void OnStingHit(class UPrimitiveComponent * HitComponent, AActor * OtherActor, class UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit);

private:
	bool ActorIsCrypteckBall(const TArray<FName> & Tags);

public:	
	// Sets default values for this actor's properties
	ASting();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(const FVector & NewDirection, bool CrypteckAmmunition = true);

	void ConvertToAmmo();

	virtual void OnBeingSucked(const class UDustpan & Dustpan) override;

	FORCEINLINE bool CanBeSucked() { return Pickable; }

};
