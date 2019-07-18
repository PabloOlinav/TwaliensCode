// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class TWALIENS_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal")
	class UStaticMeshComponent * PortalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal")
	class UBoxComponent * PortalTrigger;

	bool CanTeleport = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	bool TwoWayPortal = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	float TeleportCooldown = 3.f;

	FTimerHandle CDTimer;

	UFUNCTION()
	void RestoreTeleport();

	FScriptDelegate OnComponentBeginOverlapDelegate;

	UFUNCTION()
	void DetectIfPlayerEnter(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	class ACharacterManager * PlayerManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	APortal * OtherPortal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	FVector OtherPortalTeleportOffset = FVector(100, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	FVector OtherPlayersOffset = FVector(0, 100, 0);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
