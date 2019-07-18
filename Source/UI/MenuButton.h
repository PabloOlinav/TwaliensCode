// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Button.h"
#include "MenuButton.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UMenuButton : public UButton
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FButtonStyle MouseStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FButtonStyle KeyStyle;

	FScriptDelegate EventPressed;
	FScriptDelegate EventReleased;

public:
	UMenuButton();
	~UMenuButton();

	UFUNCTION(BlueprintCallable)
		void CheckFocus();

	UFUNCTION(BlueprintCallable)
		void PressedScale();

	UFUNCTION(BlueprintCallable)
		void ReleasedScale();
};
