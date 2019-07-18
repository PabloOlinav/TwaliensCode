// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TwaliensGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UTwaliensGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	TSubclassOf<class UUserWidget> LoadScreenSubclass;
	class UWorld* world;

protected:

	void Init() override;

	UPROPERTY(EditAnywhere, Category = "Settings")
		class UUserWidget* LoadScreen;

	UPROPERTY(BlueprintReadWrite, Category = "Settings")
		FName CurrentLevel;

	FName Level;
	FScriptDelegate LevelLoadedDelegate;
	FScriptDelegate LevelUnloadedDelegate;
	class ULevelStreaming* StreamingLevel;
	class ATwaliensGameModeBase* TwaliensGameMode;

public:

	UTwaliensGameInstance();

	UFUNCTION(BlueprintCallable)
		void ChangeLevel(FName LevelName);

	UFUNCTION(BlueprintCallable)
		void LevelUnloaded();

	UFUNCTION(BlueprintCallable)
		void LevelLoaded();

	UFUNCTION(BlueprintCallable)
		void DisableSplitscreen(bool Value);

	UFUNCTION(BlueprintCallable)
		void ZeroPlayerPositions();
};

