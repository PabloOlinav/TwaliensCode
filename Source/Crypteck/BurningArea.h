// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrypteckArea.h"
#include "BurningArea.generated.h"

UCLASS()
class TWALIENS_API ABurningArea : public ACrypteckArea
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABurningArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnSpecificConstruct(float Percentage) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
