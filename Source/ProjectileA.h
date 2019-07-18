// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interfaces/IPooledObject.h"
#include "ProjectileA.generated.h"


UCLASS()
class TWALIENS_API AProjectileA : public AActor, public IIPooledObject
{
	GENERATED_BODY()

private:
	FTimerHandle LifeTimeHanddle;
	float LifeTime;

public:	
	// Sets default values for this actor's properties
	AProjectileA();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyHit(class UPrimitiveComponent * MyComp, AActor * Other, class UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit) override;

	//Sphere collision component
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	float damage;

	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	TArray<FName> AllowedTagsToDamage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function that initializes the projectile's velocity in the shoot direction.
	UFUNCTION()
	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION()
	void AddAllowedTagsToDamage(FName TagName);

	UFUNCTION()
	void ResetAllowedTagsToDamage();

	UFUNCTION()
	virtual void OnObjectExitsPool() override;

	UFUNCTION()
	virtual void OnObjectEnterPool() override;

};
