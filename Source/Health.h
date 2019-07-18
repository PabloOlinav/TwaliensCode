// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Health.generated.h"

UCLASS()
class TWALIENS_API AHealth : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
	class UBoxComponent* HealthCollider;

	//Object Mesh
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class UStaticMeshComponent* HealthMesh;

	//Object Type
	UPROPERTY(EditAnywhere, Category = "Object Settings")
	float HealthAmount = 15;

	//Tags Allowed to Interact
	UPROPERTY(EditAnywhere, Category = "Object Settings")
	TArray<FName> AllowedTagsToInteract;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Called when overlap with valid actors
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
