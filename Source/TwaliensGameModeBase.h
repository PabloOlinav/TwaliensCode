// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TwaliensGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class TWALIENS_API ATwaliensGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	int MaxPlayerCount = 2;
	
	//TSubclassOf<APawn> Player;

	// Sets default values for this character's properties
	ATwaliensGameModeBase();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//void FindRespawnLocations();

	//void SpawnPlayers();
};
