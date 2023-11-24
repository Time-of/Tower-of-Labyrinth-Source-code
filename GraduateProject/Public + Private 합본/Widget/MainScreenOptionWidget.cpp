
#include "MainScreenOptionWidget.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/Option/OptionBlockBase.h"
#include "Widget/Option/OptionPage_Controls.h"
#include "Widget/Option/OptionPage_Graphics.h"
#include "Widget/Option/PanelButton.h"

#include <GraduateProject/GraduateProject.h>


UMainScreenOptionWidget::UMainScreenOptionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}


void UMainScreenOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	bCanSaveChanges = true;

	SetButtonKeyTexts();
	_OnPanelButtonHovered(PanelButton_Controls);
	_OnPanelButtonSelected(PanelButton_Controls);

	PanelButton_Controls->Button_->OnClicked.AddUniqueDynamic(this, &UMainScreenOptionWidget::SwitchToControlsPanel);
	PanelButton_Controls->Button_->OnHovered.AddUniqueDynamic(this, &UMainScreenOptionWidget::OnControlPanelButtonHovered);
	PanelButton_Graphics->Button_->OnClicked.AddUniqueDynamic(this, &UMainScreenOptionWidget::SwitchToGraphicsPanel);
	PanelButton_Graphics->Button_->OnHovered.AddUniqueDynamic(this, &UMainScreenOptionWidget::OnGraphicsPanelButtonHovered);
	SaveChangesButton->OnClicked.AddUniqueDynamic(this, &UMainScreenOptionWidget::OnSaveChangesButtonClicked);
}


void UMainScreenOptionWidget::NativeDestruct()
{
	Super::NativeDestruct();

	PanelButton_Controls->Button_->OnClicked.RemoveDynamic(this, &UMainScreenOptionWidget::SwitchToControlsPanel);
	PanelButton_Controls->Button_->OnHovered.RemoveDynamic(this, &UMainScreenOptionWidget::OnControlPanelButtonHovered);
	PanelButton_Graphics->Button_->OnClicked.RemoveDynamic(this, &UMainScreenOptionWidget::SwitchToGraphicsPanel);
	PanelButton_Graphics->Button_->OnHovered.RemoveDynamic(this, &UMainScreenOptionWidget::OnGraphicsPanelButtonHovered);
	SaveChangesButton->OnClicked.RemoveDynamic(this, &UMainScreenOptionWidget::OnSaveChangesButtonClicked);
}


FReply UMainScreenOptionWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	check(!InteractMapping.IsEmpty());
	check(!EscapeMapping.IsEmpty());

	if (InKeyEvent.GetKey() == InteractMapping[0].Key)
	{
		UE_LOG(LogMainScreen, Log, TEXT("메인 스크린 옵션: 확인 액션"));

		OnSaveChangesButtonClicked();

		return FReply::Handled();
	}
	// Escape 입력을 받은 경우
	else if (InKeyEvent.GetKey() == EscapeMapping[0].Key)
	{
		UE_LOG(LogMainScreen, Log, TEXT("메인 스크린 옵션: 취소 액션"));

		ReturnToMainScreenButton->OnClicked.Broadcast();

		return FReply::Handled();
	}

	return FReply::Unhandled();
}


void UMainScreenOptionWidget::OnSwitchedToThis()
{
	SetKeyboardFocus();
}


void UMainScreenOptionWidget::SetButtonKeyTexts()
{
	UInputSettings::GetInputSettings()->GetActionMappingByName(TEXT("Interact"), InteractMapping);
	UInputSettings::GetInputSettings()->GetActionMappingByName(TEXT("Escape"), EscapeMapping);
	check(!InteractMapping.IsEmpty());
	check(!EscapeMapping.IsEmpty());

	SaveButtonKeyText->SetText(FText::FromString(TEXT("[") + InteractMapping[0].Key.ToString() + TEXT("]")));
	ReturnButtonKeyText->SetText(FText::FromString(TEXT("[") + EscapeMapping[0].Key.ToString() + TEXT("]")));
}


void UMainScreenOptionWidget::SwitchToControlsPanel()
{
	OptionPageSwitcher->SetActiveWidget(OptionPage_Controls);
	_OnPanelButtonSelected(PanelButton_Controls);
}


void UMainScreenOptionWidget::SwitchToGraphicsPanel()
{
	OptionPageSwitcher->SetActiveWidget(OptionPage_Graphics);
	_OnPanelButtonSelected(PanelButton_Graphics);
}


void UMainScreenOptionWidget::OnSaveChangesButtonClicked()
{
	OptionPage_Controls->ApplyAndSaveSettings();
	OptionPage_Graphics->ApplyGraphicSettings();
	PostSaveChangedsButtonClicked();
}


void UMainScreenOptionWidget::OnControlPanelButtonHovered()
{
	_OnPanelButtonHovered(PanelButton_Controls);
}


void UMainScreenOptionWidget::OnGraphicsPanelButtonHovered()
{
	_OnPanelButtonHovered(PanelButton_Graphics);
}


void UMainScreenOptionWidget::_OnPanelButtonHovered(TObjectPtr<UPanelButton> button)
{
	if (IsValid(PanelButton_CurrentHovered)) PanelButton_CurrentHovered->SetRenderOpacity(0.5f);
	PanelButton_CurrentHovered = button;
	PanelButton_CurrentHovered->SetRenderOpacity(1.0f);
}


void UMainScreenOptionWidget::_OnPanelButtonSelected(TObjectPtr<UPanelButton> button)
{
	if (IsValid(PanelButton_CurrentClicked)) PanelButton_CurrentClicked->SetColorAndOpacity(FLinearColor::White);
	PanelButton_CurrentClicked = button;
	PanelButton_CurrentClicked->SetColorAndOpacity(FLinearColor(1.0f, 0.8f, 0.18f, 1.0f));
}
