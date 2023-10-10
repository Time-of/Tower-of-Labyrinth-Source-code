
#include "Widget/InteractableNPCWidgetBase.h"

#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"



UInteractableNPCWidgetBase::UInteractableNPCWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}



void UInteractableNPCWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetKeyboardFocus();
}



FReply UInteractableNPCWidgetBase::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	TArray<FInputActionKeyMapping> OutMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName(TEXT("Escape"), OutMappings);
	
	if (!OutMappings.IsValidIndex(0)) return FReply::Unhandled();

	if (InKeyEvent.GetKey() == OutMappings[0].Key)
	{
		RemoveThisFromParent();

		return FReply::Handled();
	}

	return FReply::Unhandled();
}



void UInteractableNPCWidgetBase::RemoveThisFromParent()
{
	if (IsInViewport()) RemoveFromParent();
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (IsValid(PC))
	{
		PC->SetShowMouseCursor(false);
		PC->SetViewTargetWithBlend(PC->GetPawn(), 0.2f, VTBlend_Cubic, 3.0f);

		FInputModeGameOnly GameOnlyInputMode;

		PC->SetInputMode(GameOnlyInputMode);
	}
}
