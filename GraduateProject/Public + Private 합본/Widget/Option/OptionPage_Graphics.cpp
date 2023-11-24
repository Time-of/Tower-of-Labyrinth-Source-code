
#include "Widget/Option/OptionPage_Graphics.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Widget/Option/OptionBlockBase.h"
#include "Widget/Option/OptionBlock_DropDown.h"
#include "Widget/Option/OptionBlock_Toggle.h"
#include <GraduateProject/GraduateProject.h>


UOptionPage_Graphics::UOptionPage_Graphics(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	frameRateLimitMap.Empty();
	frameRateLimitMap.Reserve(10);
	frameRateLimitMap.Add(0, 30.0f);
	frameRateLimitMap.Add(1, 60.0f);
	frameRateLimitMap.Add(2, 120.0f);
	frameRateLimitMap.Add(3, 144.0f);
	frameRateLimitMap.Add(4, 160.0f);
	frameRateLimitMap.Add(5, 165.0f);
	frameRateLimitMap.Add(6, 180.0f);
	frameRateLimitMap.Add(7, 200.0f);
	frameRateLimitMap.Add(8, 240.0f);
	frameRateLimitMap.Add(9, 360.0f);
}


void UOptionPage_Graphics::NativeConstruct()
{
	Super::NativeConstruct();

	SetComboBoxValues();
	LoadGraphicSettingsInfo();

	D_ScreenMode->DropDownMenu->OnSelectionChanged.AddUniqueDynamic(this, &UOptionPage_Graphics::OnScreenModeDropdownChanged);
	D_Resolution->DropDownMenu->OnSelectionChanged.AddUniqueDynamic(this, &UOptionPage_Graphics::OnResolutionDropdownChanged);
	D_MaxFrameLimit->DropDownMenu->OnSelectionChanged.AddUniqueDynamic(this, &UOptionPage_Graphics::OnMaxFrameLimitDropdownChanged);
	T_VSync->SettingToggleButton->OnClicked.AddUniqueDynamic(this, &UOptionPage_Graphics::OnVSyncButtonClicked);
	D_OverallQuality->DropDownMenu->OnSelectionChanged.AddUniqueDynamic(this, &UOptionPage_Graphics::OnOverallQualityChanged);
}


void UOptionPage_Graphics::NativeDestruct()
{
	Super::NativeDestruct();

	D_ScreenMode->DropDownMenu->OnSelectionChanged.RemoveDynamic(this, &UOptionPage_Graphics::OnScreenModeDropdownChanged);
	D_Resolution->DropDownMenu->OnSelectionChanged.RemoveDynamic(this, &UOptionPage_Graphics::OnResolutionDropdownChanged);
	D_MaxFrameLimit->DropDownMenu->OnSelectionChanged.RemoveDynamic(this, &UOptionPage_Graphics::OnMaxFrameLimitDropdownChanged);
	T_VSync->SettingToggleButton->OnClicked.RemoveDynamic(this, &UOptionPage_Graphics::OnVSyncButtonClicked);
	D_OverallQuality->DropDownMenu->OnSelectionChanged.RemoveDynamic(this, &UOptionPage_Graphics::OnOverallQualityChanged);
}


void UOptionPage_Graphics::ApplyGraphicSettings()
{
	auto settings = UGameUserSettings::GetGameUserSettings();

	settings->SetGlobalIlluminationQuality(D_GlobalIlluminationQuality->DropDownMenu->GetSelectedIndex());
	settings->SetTextureQuality(D_TextureQuality->DropDownMenu->GetSelectedIndex());
	settings->SetShadowQuality(D_ShadowQuality->DropDownMenu->GetSelectedIndex());
	settings->SetVisualEffectQuality(D_EffectQuality->DropDownMenu->GetSelectedIndex());
	settings->SetAntiAliasingQuality(D_AntialiasingQuality->DropDownMenu->GetSelectedIndex());
	settings->SetPostProcessingQuality(D_PostprocessingQuality->DropDownMenu->GetSelectedIndex());
	settings->SetShadingQuality(D_ShadingQuality->DropDownMenu->GetSelectedIndex());
	settings->SetReflectionQuality(D_ReflectionQuality->DropDownMenu->GetSelectedIndex());
	settings->SetFoliageQuality(D_FoliageQuality->DropDownMenu->GetSelectedIndex());
	settings->SetViewDistanceQuality(D_ViewDistanceQuality->DropDownMenu->GetSelectedIndex());

	settings->ApplyNonResolutionSettings();
	settings->SaveSettings();
}


void UOptionPage_Graphics::SetComboBoxValues()
{
	D_ScreenMode->DropDownMenu->ClearOptions();
	D_ScreenMode->DropDownMenu->AddOption(TEXT("FULLSCREEN"));
	D_ScreenMode->DropDownMenu->AddOption(TEXT("BORDERLESS WINDOWED"));
	D_ScreenMode->DropDownMenu->AddOption(TEXT("WINDOWED"));

	// OnSelectionChanged -> (FString, ESelectInfo)
	D_Resolution->DropDownMenu->ClearOptions();
	D_Resolution->DropDownMenu->AddOption(TEXT("1136x640"));
	D_Resolution->DropDownMenu->AddOption(TEXT("1280x720"));
	D_Resolution->DropDownMenu->AddOption(TEXT("1366x768"));
	D_Resolution->DropDownMenu->AddOption(TEXT("1600x900"));
	D_Resolution->DropDownMenu->AddOption(TEXT("1920x1080"));
	D_Resolution->DropDownMenu->AddOption(TEXT("1920x1200"));
	D_Resolution->DropDownMenu->AddOption(TEXT("2048x1536"));
	D_Resolution->DropDownMenu->AddOption(TEXT("2560x1080"));
	D_Resolution->DropDownMenu->AddOption(TEXT("2560x1440"));
	D_Resolution->DropDownMenu->AddOption(TEXT("2560x1600"));

	D_MaxFrameLimit->DropDownMenu->ClearOptions();
	for (int i = 0; i <= 9; ++i)
	{
		D_MaxFrameLimit->DropDownMenu->AddOption(FString::FromInt(FMath::RoundToInt(frameRateLimitMap[i])));
	}

	D_OverallQuality->DropDownMenu->AddOption(TEXT("사용자 지정"));
}


void UOptionPage_Graphics::LoadGraphicSettingsInfo()
{
	auto settings = UGameUserSettings::GetGameUserSettings();

	D_ScreenMode->DropDownMenu->SetSelectedIndex(static_cast<int>(settings->GetFullscreenMode()));
	D_Resolution->DropDownMenu->SetSelectedOption(TEXT("1920x1080"));
	D_MaxFrameLimit->DropDownMenu->SetSelectedOption(FString::FromInt(FMath::RoundToInt(settings->GetFrameRateLimit())));
	T_VSync->SetCurrentState(settings->IsVSyncEnabled());

	D_OverallQuality->DropDownMenu->SetSelectedOption(TEXT("사용자 지정"));
	D_GlobalIlluminationQuality->DropDownMenu->SetSelectedIndex(settings->GetGlobalIlluminationQuality());
	D_TextureQuality->DropDownMenu->SetSelectedIndex(settings->GetTextureQuality());
	D_ShadowQuality->DropDownMenu->SetSelectedIndex(settings->GetShadowQuality());
	D_EffectQuality->DropDownMenu->SetSelectedIndex(settings->GetVisualEffectQuality());
	D_AntialiasingQuality->DropDownMenu->SetSelectedIndex(settings->GetAntiAliasingQuality());
	D_PostprocessingQuality->DropDownMenu->SetSelectedIndex(settings->GetPostProcessingQuality());
	D_ShadingQuality->DropDownMenu->SetSelectedIndex(settings->GetShadingQuality());
	D_ReflectionQuality->DropDownMenu->SetSelectedIndex(settings->GetReflectionQuality());
	D_FoliageQuality->DropDownMenu->SetSelectedIndex(settings->GetFoliageQuality());
	D_ViewDistanceQuality->DropDownMenu->SetSelectedIndex(settings->GetViewDistanceQuality());
}


void UOptionPage_Graphics::OnScreenModeDropdownChanged(FString OptionName, ESelectInfo::Type SelectInfo)
{
	auto settings = UGameUserSettings::GetGameUserSettings();
	settings->SetFullscreenMode(EWindowMode::ConvertIntToWindowMode(D_ScreenMode->DropDownMenu->GetSelectedIndex()));
	settings->ApplySettings(false);
	UE_LOG(LogMainScreen, Log, TEXT("OptionPage_Graphics: 스크린 모드 설정: %d"), static_cast<int>(settings->GetFullscreenMode()));
}


void UOptionPage_Graphics::OnResolutionDropdownChanged(FString OptionName, ESelectInfo::Type SelectInfo)
{
	auto settings = UGameUserSettings::GetGameUserSettings();
	FString left;
	FString right;
	UKismetStringLibrary::Split(OptionName, TEXT("x"), left, right, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	FIntPoint goalResolution(UKismetStringLibrary::Conv_StringToInt(left), UKismetStringLibrary::Conv_StringToInt(right));

	settings->SetScreenResolution(goalResolution);
	settings->ApplySettings(false);
	UE_LOG(LogMainScreen, Log, TEXT("OptionPage_Graphics: 해상도 설정: %d x %d"), settings->GetScreenResolution().X, settings->GetScreenResolution().Y);
}


void UOptionPage_Graphics::OnMaxFrameLimitDropdownChanged(FString OptionName, ESelectInfo::Type SelectInfo)
{
	auto settings = UGameUserSettings::GetGameUserSettings();
	settings->SetFrameRateLimit(frameRateLimitMap[D_MaxFrameLimit->DropDownMenu->GetSelectedIndex()]);
	settings->ApplySettings(false);
	UE_LOG(LogMainScreen, Log, TEXT("OptionPage_Graphics: MaxFrameLimit 설정: %f"), settings->GetFrameRateLimit());
}


void UOptionPage_Graphics::OnVSyncButtonClicked()
{
	auto settings = UGameUserSettings::GetGameUserSettings();
	settings->SetVSyncEnabled(T_VSync->ToggleButton());
	settings->ApplySettings(false); 
	UE_LOG(LogMainScreen, Log, TEXT("OptionPage_Graphics: VSync 설정: %d"), settings->IsVSyncEnabled() ? 1 : 0);
}


void UOptionPage_Graphics::OnOverallQualityChanged(FString OptionName, ESelectInfo::Type SelectInfo)
{
	int selectedIndex = D_OverallQuality->DropDownMenu->GetSelectedIndex();
	UE_LOG(LogMainScreen, Log, TEXT("OptionPage_Graphics: 전역 그래픽 설정 선택된 인덱스: %d"), selectedIndex);
	if (selectedIndex >= 5) return;
	auto settings = UGameUserSettings::GetGameUserSettings();
	settings->SetOverallScalabilityLevel(selectedIndex);
	settings->ApplySettings(false);
	UE_LOG(LogMainScreen, Log, TEXT("OptionPage_Graphics: 전역 그래픽 품질 설정: %d"), settings->GetOverallScalabilityLevel());
	LoadGraphicSettingsInfo();
	UE_LOG(LogMainScreen, Log, TEXT("OptionPage_Graphics: 전역 그래픽 설정 Load 후 몇일까: %d"), settings->GetOverallScalabilityLevel());
}
