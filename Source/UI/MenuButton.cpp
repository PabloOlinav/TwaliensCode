// All rights reserved.

#include "MenuButton.h"
#include "ConstructorHelpers.h"

UMenuButton::UMenuButton()
{
	ConstructorHelpers::FObjectFinder<UTexture> ButtonImageNormal(TEXT("/Game/Materials/UserInterface/Interface_Main_Menu_01/T_TextureBotonNewGame_02"));

	ConstructorHelpers::FObjectFinder<UTexture> ButtonImageHover(TEXT("/Game/Materials/UserInterface/Interface_Main_Menu_01/T_TextureBotonAreNoPRESSED_02"));
	
	FVector Color = FVector(0.0f, 1.0f, 0.212231f);

	UObject* NormalImage = Cast<UObject>(ButtonImageNormal.Object);
	UObject* HoverImage = Cast<UObject>(ButtonImageHover.Object);

	MouseStyle.Normal = WidgetStyle.Normal;
	MouseStyle.Hovered = WidgetStyle.Hovered;
	MouseStyle.Pressed = WidgetStyle.Pressed;

	MouseStyle.Normal.SetResourceObject(NormalImage);
	MouseStyle.Hovered.SetResourceObject(HoverImage);
	MouseStyle.Pressed.SetResourceObject(NormalImage);

	MouseStyle.Normal.DrawAs = ESlateBrushDrawType::Image;
	MouseStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;
	MouseStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;

	KeyStyle.Normal = WidgetStyle.Normal;
	KeyStyle.Hovered = WidgetStyle.Hovered;
	KeyStyle.Pressed = WidgetStyle.Pressed;

	KeyStyle.Normal.SetResourceObject(HoverImage);
	KeyStyle.Hovered.SetResourceObject(HoverImage);
	KeyStyle.Pressed.SetResourceObject(NormalImage);

	KeyStyle.Normal.DrawAs = ESlateBrushDrawType::Image;
	KeyStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;
	KeyStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;

	//MouseStyle.Hovered.TintColor = FLinearColor(Color);
	//KeyStyle.Normal.TintColor = FLinearColor(Color);
	//KeyStyle.Hovered.TintColor = FLinearColor(Color);
	//KeyStyle.Pressed.TintColor = FLinearColor(Color);

	EventPressed.BindUFunction(this, FName("PressedScale"));
	OnPressed.Add(EventPressed);

	EventReleased.BindUFunction(this, FName("ReleasedScale"));
	OnReleased.Add(EventReleased);
}

UMenuButton::~UMenuButton()
{
}

void UMenuButton::CheckFocus()
{
	if(IsHovered())
	{
		SetKeyboardFocus();
		SetStyle(MouseStyle);
	}
	else
	{
		if(HasKeyboardFocus())
		{
			SetStyle(KeyStyle);
		}
		else
		{
			SetStyle(MouseStyle);
		}
	}
}

void UMenuButton::PressedScale()
{
	SetRenderScale(FVector2D(0.8f, 0.8f));
}

void UMenuButton::ReleasedScale()
{
	SetRenderScale(FVector2D(1.0f, 1.0f));
}

