// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TWaliensDisplayUI.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API ATWaliensDisplayUI : public AHUD
{
	GENERATED_BODY()

private:
	TSubclassOf<class UUserWidget> PlayerWidgetClass;
	TSubclassOf<class UUserWidget> ControlZoneWidgetClass;
	//TSubclassOf<class UUserWidget> PlayerHabilitiesUIClass;

	class UUserWidget* PlayerUI;
	class UUserWidget* ControlZoneUI;
	//class UUserWidget* HabilitiesUI;

public:
	ATWaliensDisplayUI();

	UFUNCTION(BlueprintCallable)
	void SetVisibility(bool NewState);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
