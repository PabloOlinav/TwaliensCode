// All rights reserved.

#include "TWaliensDisplayUI.h"
#include "UserWidget.h"
#include "ConstructorHelpers.h"

ATWaliensDisplayUI::ATWaliensDisplayUI()
{
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerWidgetAsset(TEXT("/Game/Interface/Player_UI"));
	ConstructorHelpers::FClassFinder<UUserWidget> ControlZoneWidgetAsset(TEXT("/Game/Interface/BP_ControlZone_UI"));
	//ConstructorHelpers::FClassFinder<UUserWidget> Habilities(TEXT("/Game/Interface/Habilities_UI"));

	if (PlayerWidgetAsset.Succeeded())
	{
		PlayerWidgetClass = PlayerWidgetAsset.Class;
	}

	//if(ControlZoneWidgetAsset.Succeeded())
	//{
	//	ControlZoneWidgetClass = ControlZoneWidgetAsset.Class;
	//}

	/*if (Habilities.Succeeded())
	{
		PlayerHabilitiesUIClass = Habilities.Class;

	}*/
}


void ATWaliensDisplayUI::SetVisibility(bool NewState)
{
	if (NewState)
	{
		//PlayerUI->SetVisibility(ESlateVisibility::Visible);
		//ControlZoneUI->SetVisibility(ESlateVisibility::Visible);
		//HabilitiesUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PlayerUI->SetVisibility(ESlateVisibility::Hidden);
		//ControlZoneUI->SetVisibility(ESlateVisibility::Hidden);
		//HabilitiesUI->SetVisibility(ESlateVisibility::Hidden);
	}

}

void ATWaliensDisplayUI::BeginPlay()
{
	UWorld * const world = GetWorld();

	if (world)
	{
		//if (PlayerWidgetClass)
		//{
		//	PlayerUI = CreateWidget<UUserWidget>(world, PlayerWidgetClass);

		//	if (PlayerUI)
		//	{
		//		PlayerUI->AddToViewport();
		//	}
		//}

		//if(ControlZoneWidgetClass)
		//{
		//	ControlZoneUI = CreateWidget<UUserWidget>(world, ControlZoneWidgetClass);

		//	if(ControlZoneUI)
		//	{
		//		ControlZoneUI->AddToViewport();
		//	}
		//}

		/*if (PlayerHabilitiesUIClass != NULL)
		{
			HabilitiesUI = CreateWidget<UUserWidget>(world, PlayerHabilitiesUIClass);

			if (HabilitiesUI != NULL)
				HabilitiesUI->AddToViewport();
		}*/
	}
}
