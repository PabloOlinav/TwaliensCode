// Fill out your copyright notice in the Description page of Project Settings.

#include "GameViewportDivisble.h"

void UGameViewportDivisble::UpdateActiveSplitscreenType()
{
	ActiveSplitscreenType = CurrentScreenState;
}

void UGameViewportDivisble::CancelSplitscreen()
{
	CurrentScreenState = ESplitScreenType::None;
}

void UGameViewportDivisble::BeginSplitscreen()
{
	if (IsVerticalOriented)
		CurrentScreenState = ESplitScreenType::TwoPlayer_Vertical;
	else
		CurrentScreenState = ESplitScreenType::TwoPlayer_Horizontal;
	//SetDisableSplitscreenOverride(false);
}

void UGameViewportDivisble::SetVerticalOrientation()
{
	IsVerticalOriented = true;
}

void UGameViewportDivisble::SetHorizontalOrientation()
{
	IsVerticalOriented = false;
}
