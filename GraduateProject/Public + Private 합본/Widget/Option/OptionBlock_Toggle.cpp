
#include "Widget/Option/OptionBlock_Toggle.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"


bool UOptionBlock_Toggle::ToggleButton()
{
	SetCurrentState(!bIsActivated);

	return bIsActivated;
}


void UOptionBlock_Toggle::SetCurrentState(bool bIsOn)
{
	if (bIsActivated == bIsOn) return;

	bIsActivated = bIsOn;

	CurrentStateText->SetText(FText::FromString(bIsActivated ? TEXT("On") : TEXT("Off")));
}
