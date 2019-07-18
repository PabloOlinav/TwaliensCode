// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerNumberIndex.h"
#include "VoronoiCameraActor.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;
class UPostProcessComponent;



UCLASS()
class TWALIENS_API AVoronoiCameraActor : public AActor
{
	GENERATED_BODY()

private:
	APlayerController* Player;
	
public:	
	// Sets default values for this actor's properties
	AVoronoiCameraActor();

	// Components
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		UCameraComponent* CameraComponnt;
	UPROPERTY(BlueprintReadWrite)
		USpringArmComponent* SpringArmComponnt;
	UPROPERTY(BlueprintReadWrite)
		UStaticMeshComponent* StaticMeshComponnt;
	

	UPROPERTY(EditAnywhere, Category = "Enum")
		EPlayerNumberIndex PlayerEnum;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "PlayerID")
		int32 FollowedPlayerIndex;

	//Properties
	UPROPERTY(EditAnywhere, category = "Settings")
		float TrgtArmLength;

	UPROPERTY(EditAnywhere, category = "Settings")
		FRotator RotationAngles;

	UPROPERTY(BlueprintReadOnly,EditAnywhere, category = "Settings")
		float FOV = 45.f;

	/*UPROPERTY(EditAnywhere, category = "Settings")
		float MaxDistanceTrgtArmLength;
*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Settings")
		FRotator OffsetRotationAngles;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "PostProcessing")
		UPostProcessComponent* PostProcessingComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};


