
#include "Widget/Option/PanelButton.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UPanelButton::K2_SetButtonText(FText NewText)
{
	ButtonText->SetText(NewText);
}


void UPanelButton::K2_SetIconImage(UTexture2D* NewTexture)
{
	Icon->SetBrushFromTexture(NewTexture, false);
}