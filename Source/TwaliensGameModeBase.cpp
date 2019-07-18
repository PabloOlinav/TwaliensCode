// Fill out your copyright notice in the Description page of Project Settings.

#include "TwaliensGameModeBase.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "TWaliensDisplayUI.h"


ATwaliensGameModeBase::ATwaliensGameModeBase()
{
	//DefaultPawnClass = ATwalien::StaticClass();
	DefaultPawnClass = NULL;
	HUDClass = ATWaliensDisplayUI::StaticClass();
}

void ATwaliensGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//FindRespawnLocations();
}
