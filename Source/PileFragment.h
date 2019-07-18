// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SuckableObjects.h"
#include "Interfaces/IPooledObject.h"
#include "PileFragment.generated.h"

class UParticleSystemComponent;

UCLASS()
class TWALIENS_API APileFragment : public AActor, public ISuckableObjects, public IIPooledObject
{
	GENERATED_BODY()

private:
	class UMaterialInstanceDynamic* Fresnel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
		class UBoxComponent* FragmentCollider;

	//Object Mesh
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
		class UStaticMeshComponent* FragmentMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particules")
		UParticleSystemComponent* PS_PileFragment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particules")
		class USphereComponent* SphereComponent;

	FVector DestinationLocation;

	FScriptDelegate SphereComponentBeginOverlap;
	FScriptDelegate SphereComponentEndOverlap;

	UFUNCTION()
		void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnSphereComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:	

	// Sets default values for this actor's properties
	APileFragment();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnItemBecomeDisable();

	void OnItemBecomeEnable();

	virtual void OnBeingSucked(const class UDustpan & Dustpan) override;

	UFUNCTION(BlueprintCallable)
		virtual void OnObjectExitsPool() override;

	virtual void OnObjectEnterPool() override;

	FORCEINLINE void SetDestinationLocation(const FVector & Destination) { DestinationLocation = Destination; }
	FORCEINLINE UStaticMeshComponent* GetFragmentMesh() { return FragmentMesh; }
};
