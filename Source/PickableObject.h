// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableObject.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EObjectType : uint8
{
	Type_Health UMETA(DisplayName = "Health"),
	Type_Ammunition UMETA(DisplayName = "Ammunition"),
};

UCLASS()
class TWALIENS_API APickableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
		class USphereComponent* CollisionComp;

	//Object Mesh
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
		UStaticMeshComponent* ObjectMesh;

	//Object Type
	UPROPERTY(EditAnywhere, Category = "Object Settings")
		EObjectType ObjectType;

	//Object Type
	UPROPERTY(EditAnywhere, Category = "Object Settings")
		float RechargeAmmount;

	//Tags Allowed to Interact
	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
		TArray<FName> AllowedTagsToInteract;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Called when overlap with valid actors
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
