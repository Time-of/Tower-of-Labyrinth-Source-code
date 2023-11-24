
#include "Widget/MainScreenCreditWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include <GraduateProject/GraduateProject.h>


UMainScreenCreditWidget::UMainScreenCreditWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}


void UMainScreenCreditWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ReturnToMainScreenButton->OnClicked.AddUniqueDynamic(this, &UMainScreenCreditWidget::OnReturnButtonClicked);
}


void UMainScreenCreditWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ReturnToMainScreenButton->OnClicked.RemoveDynamic(this, &UMainScreenCreditWidget::OnReturnButtonClicked);
}


FReply UMainScreenCreditWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	TArray<FInputActionKeyMapping> OutActionMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName(TEXT("Escape"), OutActionMappings);
	check(!OutActionMappings.IsEmpty());

	if (InKeyEvent.GetKey() == OutActionMappings[0].Key)
	{
		UE_LOG(LogMainScreen, Log, TEXT("크레딧: 취소 액션"));

		OnReturnButtonClicked();

		return FReply::Handled();
	}

	return FReply::Unhandled();
}


void UMainScreenCreditWidget::OnSwitchedToThis()
{
	SetKeyboardFocus();
}


void UMainScreenCreditWidget::OnReturnButtonClicked()
{
	UE_LOG(LogMainScreen, Log, TEXT("크레딧: 뒤로 돌아가기 버튼 클릭"));
	OnReturnToMainScreenButtonClickedDelegate.Broadcast();
}
