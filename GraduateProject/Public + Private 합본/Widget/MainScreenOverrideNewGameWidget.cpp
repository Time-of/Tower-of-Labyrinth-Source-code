
#include "Widget/MainScreenOverrideNewGameWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "BlueprintFunctionLibrary/DataSaveLoadHelper.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include <GraduateProject/GraduateProject.h>


UMainScreenOverrideNewGameWidget::UMainScreenOverrideNewGameWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}


void UMainScreenOverrideNewGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	YesButton->OnClicked.AddUniqueDynamic(this, &UMainScreenOverrideNewGameWidget::OnYesButtonClicked);
	NoButton->OnClicked.AddUniqueDynamic(this, &UMainScreenOverrideNewGameWidget::OnNoButtonClicked);
}


void UMainScreenOverrideNewGameWidget::NativeDestruct()
{
	Super::NativeDestruct();

	YesButton->OnClicked.RemoveDynamic(this, &UMainScreenOverrideNewGameWidget::OnYesButtonClicked);
	NoButton->OnClicked.RemoveDynamic(this, &UMainScreenOverrideNewGameWidget::OnNoButtonClicked);
}


FReply UMainScreenOverrideNewGameWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	TArray<FInputActionKeyMapping> OutActionMappings;
	TArray<FInputAxisKeyMapping> OutAxisMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName(TEXT("Escape"), OutActionMappings);
	UInputSettings::GetInputSettings()->GetActionMappingByName(TEXT("Interact"), OutActionMappings);
	UInputSettings::GetInputSettings()->GetActionMappingByName(TEXT("UI_Confirm"), OutActionMappings);
	UInputSettings::GetInputSettings()->GetAxisMappingByName(TEXT("MoveRight"), OutAxisMappings);

	check(OutActionMappings.Num() >= 3);
	check(!OutAxisMappings.IsEmpty());


	// Interact나 UI_Confirm 입력을 받은 경우
	if (InKeyEvent.GetKey() == OutActionMappings[1].Key || InKeyEvent.GetKey() == OutActionMappings[2].Key)
	{
		UE_LOG(LogMainScreen, Log, TEXT("새 게임 덮어쓰기: 확인 액션"));

		if (bNowYesSelected) OnYesButtonClicked();
		else OnNoButtonClicked();

		return FReply::Handled();
	}
	// Escape 입력을 받은 경우
	else if (InKeyEvent.GetKey() == OutActionMappings[0].Key)
	{
		UE_LOG(LogMainScreen, Log, TEXT("새 게임 덮어쓰기: 취소 액션"));

		OnNoButtonClicked();

		return FReply::Handled();
	}

	return TryConsumeAxisInput(InKeyEvent, OutAxisMappings);
}


void UMainScreenOverrideNewGameWidget::OnSwitchedToThis()
{
	SelectNo();
}


void UMainScreenOverrideNewGameWidget::SelectYes()
{
	bNowYesSelected = true;
	YesSelected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	NoSelected->SetVisibility(ESlateVisibility::Hidden);
	YesButton->SetFocus();
}


void UMainScreenOverrideNewGameWidget::SelectNo()
{
	bNowYesSelected = false;
	YesSelected->SetVisibility(ESlateVisibility::Hidden);
	NoSelected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	NoButton->SetFocus();
}


void UMainScreenOverrideNewGameWidget::OnYesButtonClicked()
{
	UE_LOG(LogMainScreen, Log, TEXT("새 게임 덮어쓰기: 'Yes' 클릭하여 튜토리얼 레벨로 이동"));
	UDataSaveLoadHelper::InitializeGameData(GetOwningPlayer());
	
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer());
	GetOwningPlayer()->SetShowMouseCursor(true);

	UGameplayStatics::OpenLevel(GetOwningPlayer(), tutorialLevelName);
}


void UMainScreenOverrideNewGameWidget::OnNoButtonClicked()
{
	UE_LOG(LogMainScreen, Log, TEXT("새 게임 덮어쓰기: 'No' 버튼 클릭"));
	OnNoButtonClickedDelegate.Broadcast();
}


void UMainScreenOverrideNewGameWidget::OnAxisPositive()
{
	SelectNo();
}


void UMainScreenOverrideNewGameWidget::OnAxisNegative()
{
	SelectYes();
}
