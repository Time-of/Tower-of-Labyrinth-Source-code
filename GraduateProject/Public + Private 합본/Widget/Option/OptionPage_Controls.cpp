
#include "Widget/Option/OptionPage_Controls.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "SaveGame/SaveGame_PlayerSettings.h"
#include "Widget/Option/OptionBlockBase.h"
#include "Widget/Option/OptionBlock_Progress.h"
#include "Widget/Option/OptionBlock_Toggle.h"
#include <GraduateProject/GraduateProject.h>


void UOptionPage_Controls::NativeConstruct()
{
	Super::NativeConstruct();

	//P_CameraSensitivityX->SettingSlider->OnValueChanged.AddUniqueDynamic(this, &UOptionPage_Controls::OnCamSenXSliderValueChanged);
	//P_CameraSensitivityY->SettingSlider->OnValueChanged.AddUniqueDynamic(this, &UOptionPage_Controls::OnCamSenYSliderValueChanged);

	T_InvertMouseX->SettingToggleButton->OnClicked.AddUniqueDynamic(this, &UOptionPage_Controls::OnInvMouseXButtonClicked);
	T_InvertMouseY->SettingToggleButton->OnClicked.AddUniqueDynamic(this, &UOptionPage_Controls::OnInvMouseYButtonClicked);

	LoadSettings();
}


void UOptionPage_Controls::NativeDestruct()
{
	Super::NativeDestruct();

	//P_CameraSensitivityX->SettingSlider->OnValueChanged.RemoveDynamic(this, &UOptionPage_Controls::OnCamSenXSliderValueChanged);
	//P_CameraSensitivityY->SettingSlider->OnValueChanged.RemoveDynamic(this, &UOptionPage_Controls::OnCamSenYSliderValueChanged);

	T_InvertMouseX->SettingToggleButton->OnClicked.RemoveDynamic(this, &UOptionPage_Controls::OnInvMouseXButtonClicked);
	T_InvertMouseY->SettingToggleButton->OnClicked.RemoveDynamic(this, &UOptionPage_Controls::OnInvMouseYButtonClicked);
}


void UOptionPage_Controls::ApplyAndSaveSettings()
{
	auto sg = UGameplayStatics::CreateSaveGameObject(USaveGame_PlayerSettings::StaticClass());

	if (auto playerSettings = Cast<USaveGame_PlayerSettings>(sg))
	{
		playerSettings->mouseSensitivityX = P_CameraSensitivityX->SettingSlider->GetValue();
		playerSettings->mouseSensitivityY = P_CameraSensitivityY->SettingSlider->GetValue();
		playerSettings->bInvertMouseX = T_InvertMouseX->GetCurrentState();
		playerSettings->bInvertMouseY = T_InvertMouseY->GetCurrentState();

		UGameplayStatics::SaveGameToSlot(playerSettings, TEXT("PlayerSettings"), 0);
	}
}


void UOptionPage_Controls::LoadSettings()
{
	auto sg = UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSettings"), 0);
	auto playerSettings = Cast<USaveGame_PlayerSettings>(sg);

	if (IsValid(playerSettings))
	{
		UE_LOG(LogMainScreen, Log, TEXT("OptionPage_Controls: PlayerSettings 세이브게임 탐지 성공. 값을 불러옵니다."));

		P_CameraSensitivityX->SettingSlider->SetValue(playerSettings->mouseSensitivityX);
		P_CameraSensitivityY->SettingSlider->SetValue(playerSettings->mouseSensitivityY);
		P_CameraSensitivityX->OnSliderValueSet(playerSettings->mouseSensitivityY);
		P_CameraSensitivityY->OnSliderValueSet(playerSettings->mouseSensitivityY);
		P_CameraSensitivityX->SliderValueText->SetText(UKismetTextLibrary::Conv_FloatToText(P_CameraSensitivityX->SettingSlider->GetValue(), ERoundingMode::HalfToEven, false, true, 1, 3, 0, 1));
		P_CameraSensitivityY->SliderValueText->SetText(UKismetTextLibrary::Conv_FloatToText(P_CameraSensitivityY->SettingSlider->GetValue(), ERoundingMode::HalfToEven, false, true, 1, 3, 0, 1));
		T_InvertMouseX->SetCurrentState(playerSettings->bInvertMouseX);
		T_InvertMouseY->SetCurrentState(playerSettings->bInvertMouseY);
	}
	else
	{
		UE_LOG(LogMainScreen, Log, TEXT("OptionPage_Controls: PlayerSettings 세이브게임 탐지 실패. 기본값으로 설정합니다."));

		P_CameraSensitivityX->SettingSlider->SetValue(3.0f);
		P_CameraSensitivityY->SettingSlider->SetValue(3.0f);
		P_CameraSensitivityX->OnSliderValueSet(3.0f);
		P_CameraSensitivityY->OnSliderValueSet(3.0f);
		P_CameraSensitivityX->SliderValueText->SetText(UKismetTextLibrary::Conv_FloatToText(P_CameraSensitivityX->SettingSlider->GetValue(), ERoundingMode::HalfToEven, false, true, 1, 3, 0, 1));
		P_CameraSensitivityY->SliderValueText->SetText(UKismetTextLibrary::Conv_FloatToText(P_CameraSensitivityY->SettingSlider->GetValue(), ERoundingMode::HalfToEven, false, true, 1, 3, 0, 1));
		T_InvertMouseX->SetCurrentState(false);
		T_InvertMouseY->SetCurrentState(false);

		ApplyAndSaveSettings();
	}
}


void UOptionPage_Controls::OnInvMouseXButtonClicked()
{
	bool result = T_InvertMouseX->ToggleButton();
}


void UOptionPage_Controls::OnInvMouseYButtonClicked()
{
	bool result = T_InvertMouseY->ToggleButton();
}
