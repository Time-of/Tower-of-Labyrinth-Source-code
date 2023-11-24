
#include "Widget/Option/OptionBlockBase.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include <GraduateProject/GraduateProject.h>


UOptionBlockBase::UOptionBlockBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}


void UOptionBlockBase::K2_SetOptionNameText(FText NewText)
{
	OptionNameText->SetText(NewText);
}


void UOptionBlockBase::SetOptionNameText(const FText& NewText)
{
	OptionNameText->SetText(NewText);
}
