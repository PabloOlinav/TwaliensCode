// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrypteckBall.h"
#include "LifeComponent.h"
#include "ActiveCrypteckBall.generated.h"

UCLASS(Abstract)
class TWALIENS_API AActiveCrypteckBall : public AActor, public ICrypteckBall
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	//Constructor generated by default in generated_uclass_body macro!!!
	AActiveCrypteckBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ActiveCrypteckBall")
	class UStaticMeshComponent * Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ActiveCrypteckBall")
	class USphereComponent * Collider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "ActiveCrypteckBall")
	class ULifeComponent * Life;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveCrypteckBall")
	float DeltaTimeMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveCrypteckBall")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveCrypteckBall")
	float WidthSpace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveCrypteckBall")
	float HeightSpace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveCrypteckBall")
	float InitialAngle;

	class ACrypteck * Crypteck;

	FVector TargetPosition;
	FVector DisplacementNormal;

	FVector OrbitCenter;

	float CurrentOrbitTime;

	float Angle;

	FVector TeleportPosition;

	bool DissolveFinished;

	virtual void OnBallHit(const FHitResult & Hit) { unimplemented(); }

	virtual void CreateOrbitNewPosition(FVector & Position) { unimplemented(); }

	virtual void Move(float DeltaSeconds) { unimplemented(); }

	virtual void SetPositionToAttackTeleport() { unimplemented(); }

	void Dissolve(float DeltaSeconds);

	void Undissolve(float DeltaSeconds);

	void Orbit(float DeltaSeconds);

	void Teleport(float DeltaSeconds);

	UFUNCTION()
	void OnDeath();
	FScriptDelegate DeathDelegate;

private:
	void ConvertAngleToOrbitTime(float NewAngle);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTarget(const FVector & Target);

	void Activate() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FORCEINLINE ULifeComponent * GetSphereLifeComponent() { return Life; }
	FORCEINLINE void AddListenerToHPChanges(const FScriptDelegate & Delegate) { Life->AddDelegateToHPUpdate(Delegate); }
	FORCEINLINE void AddListenerToSphereDeath(const FScriptDelegate & Delegate) { Life->AddDelegateToDeath(Delegate); }
	FORCEINLINE void RemoveListenerToHPChanges(const FScriptDelegate & Delegate) { Life->RemoveDelegateFromHPUpdate(Delegate); }
	FORCEINLINE void RemoveListenerToSphereDeath(const FScriptDelegate & Delegate) { Life->RemoveDelegateFromDeath(Delegate); }
	FORCEINLINE void RemoveAllListenerToHPChanges() { Life->RemoveAllDelegatesFromHPUpdate(); }
	FORCEINLINE void RemoveAllListenerToSphereDeath() { Life->RemoveAllDelegatesFromDeath(); }
	FORCEINLINE void SetOrbitCenter(FVector NewOrbitCenter) { OrbitCenter = NewOrbitCenter; }
	FORCEINLINE FVector GetOrbitCenter() const { return OrbitCenter; }
	FORCEINLINE void SetInitialAngle(float NewInitialAngle) { InitialAngle = NewInitialAngle; }
	FORCEINLINE float GetInitialAngle() const { return InitialAngle; }
	FORCEINLINE bool GetBallIsInOrbit() const { return State == IDLE; }
	FORCEINLINE float GetCurrentOrbitAngle() const { return Angle; }
	FORCEINLINE void SetCryptecksReference(class ACrypteck * Reference) { Crypteck = Reference; }
};
