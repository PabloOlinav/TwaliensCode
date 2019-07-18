// All rights reserved.

#include "TwaliensGameInstance.h"
#include "UserWidget.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "ConstructorHelpers.h"
#include "EngineUtils.h"
#include "GameFramework/GameModeBase.h"
#include "TwaliensGameModeBase.h"

UTwaliensGameInstance::UTwaliensGameInstance()
{
	ConstructorHelpers::FClassFinder<UUserWidget>LoadWidget(TEXT("/Game/Interface/LoadingScreen"));

	LoadScreenSubclass = LoadWidget.Class;
}

void UTwaliensGameInstance::Init()
{
	Super::Init();

	//StreamingLevel = CreateDefaultSubobject<ULevelStreaming>(TEXT("Level"));

	LevelLoadedDelegate.BindUFunction(this, FName("LevelLoaded"));
	//StreamingLevel->OnLevelLoaded.Add(LevelLoadedDelegate);

	LevelUnloadedDelegate.BindUFunction(this, FName("LevelUnloaded"));
	//StreamingLevel->OnLevelUnloaded.Add(LevelUnloadedDelegate);
}

void UTwaliensGameInstance::ChangeLevel(FName LevelName)
{
	FLatentActionInfo LatentInfo;

	Level = LevelName;

	if(GetWorldContext())
	{
		GetGameViewportClient()->RemoveAllViewportWidgets();

		LoadScreen = CreateWidget<UUserWidget>(GetWorld(), LoadScreenSubclass);
		LoadScreen->AddToViewport();

		if (!CurrentLevel.IsNone())
		{
			StreamingLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), CurrentLevel);
			StreamingLevel->OnLevelUnloaded.Add(LevelUnloadedDelegate);
			UGameplayStatics::UnloadStreamLevel(this, CurrentLevel, LatentInfo, false);
		}
		else if(!Level.IsNone())
		{
			StreamingLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), Level);
			StreamingLevel->OnLevelLoaded.Add(LevelLoadedDelegate);
			UGameplayStatics::LoadStreamLevel(this, Level, true, false, LatentInfo);
		}
	}
}

void UTwaliensGameInstance::LevelUnloaded()
{
	FLatentActionInfo LatentInfo;

	if (GetWorldContext())
	{
		//UGameplayStatics::SetGamePaused(GetWorld(), false);

		if (!Level.IsNone())
		{
			StreamingLevel->OnLevelUnloaded.Remove(LevelUnloadedDelegate);
			StreamingLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), Level);
			StreamingLevel->OnLevelLoaded.Add(LevelLoadedDelegate);
			UGameplayStatics::LoadStreamLevel(this, Level, true, false, LatentInfo);
		}

	}
}

void UTwaliensGameInstance::LevelLoaded()
{
	CurrentLevel = Level;
	StreamingLevel->OnLevelLoaded.Remove(LevelLoadedDelegate);
	LoadScreen->RemoveFromParent();
}

void UTwaliensGameInstance::DisableSplitscreen(bool Value)
{
	if(GetWorld())
	{
		GetGameViewportClient()->SetDisableSplitscreenOverride(Value);
	}
}

void UTwaliensGameInstance::ZeroPlayerPositions()
{
	if (GetWorld())
	{

		for (TActorIterator<APlayerController> Itr(GetWorld()); Itr; ++Itr)
		{
			APawn* FoundPoint = Itr->GetPawn();

			if (FoundPoint)
			{
				FoundPoint->SetActorLocation(FVector::ZeroVector);
			}

			//TwaliensGameMode->RestartPlayer(FoundPoint);
			//FoundPoint->Reset();
			//FoundPoint->ClientReset();
		}
	}
}
