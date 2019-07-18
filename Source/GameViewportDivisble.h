// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "Engine/ViewportSplitScreen.h"
#include "GameViewportDivisble.generated.h"

/**
 * 
 */
UCLASS()
class TWALIENS_API UGameViewportDivisble : public UGameViewportClient
{
	GENERATED_BODY()

private:
	ESplitScreenType::Type CurrentScreenState;
	bool IsVerticalOriented;
	/*ESplitScreenType::Type SplitInVertical();
	ESplitScreenType::Type SplitInHorizontal();
	ESplitScreenType::Type CancelSplitting();*/

protected:
	virtual void UpdateActiveSplitscreenType() override; 

public:
	
	void CancelSplitscreen();
	void BeginSplitscreen();
	void SetVerticalOrientation();
	void SetHorizontalOrientation();
	/*void SplitInHorizontal() const;
	void CancelSplitting() const;*/

	
	
	
};
