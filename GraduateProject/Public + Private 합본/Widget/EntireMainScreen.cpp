
#include "Widget/EntireMainScreen.h"

#include "MainScreenOptionWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/MainScreenCreditWidget.h"
#include "Widget/MainScreenOverrideNewGameWidget.h"
#include "Widget/MainScreenWidget.h"
#include <GraduateProject/GraduateProject.h>


UEntireMainScreen::UEntireMainScreen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}


void UEntireMainScreen::NativeConstruct()
{
	Super::NativeConstruct();

	CheckClearInfo();
	CheckHasSaveGameData();

	MainScreenWidget->OnNewGameButtonClickedDelegate.AddUniqueDynamic(this, &UEntireMainScreen::TryStartNewGame);
	MainScreenWidget->OnOptionButtonClickedDelegate.AddUniqueDynamic(this, &UEntireMainScreen::SwitchToOptionWidget);
	MainScreenWidget->OnCreditButtonClickedDelegate.AddUniqueDynamic(this, &UEntireMainScreen::SwitchToCreditWidget);
	MainScreenOverrideNewGameWidget->OnNoButtonClickedDelegate.AddUniqueDynamic(this, &UEntireMainScreen::SwitchToMainScreenWidget);
	
	MainScreenOptionWidget->ReturnToMainScreenButton->OnClicked.AddUniqueDynamic(this, &UEntireMainScreen::SwitchToMainScreenWidget);
	MainScreenCreditWidget->OnReturnToMainScreenButtonClickedDelegate.AddUniqueDynamic(this, &UEntireMainScreen::SwitchToMainScreenWidget);
}


void UEntireMainScreen::NativeDestruct()
{
	Super::NativeDestruct();

	MainScreenWidget->OnNewGameButtonClickedDelegate.RemoveDynamic(this, &UEntireMainScreen::TryStartNewGame);
	MainScreenWidget->OnOptionButtonClickedDelegate.RemoveDynamic(this, &UEntireMainScreen::SwitchToOptionWidget);
	MainScreenWidget->OnCreditButtonClickedDelegate.RemoveDynamic(this, &UEntireMainScreen::SwitchToCreditWidget);
	MainScreenOverrideNewGameWidget->OnNoButtonClickedDelegate.RemoveDynamic(this, &UEntireMainScreen::SwitchToMainScreenWidget);

	MainScreenOptionWidget->ReturnToMainScreenButton->OnClicked.RemoveDynamic(this, &UEntireMainScreen::SwitchToMainScreenWidget);
	MainScreenCreditWidget->OnReturnToMainScreenButtonClickedDelegate.RemoveDynamic(this, &UEntireMainScreen::SwitchToMainScreenWidget);
}


void UEntireMainScreen::CheckClearInfo()
{
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("ClearInfo"), 0)) return;

	ToggleMainBackgroundButton->SetVisibility(ESlateVisibility::Visible);

	OnClearInfoExists();

	bHasBeenClearedOnce = true;
}


void UEntireMainScreen::CheckHasSaveGameData()
{
	bHasSaveGameData = UGameplayStatics::DoesSaveGameExist(TEXT("InventorySaveData"), 1);

	// 인벤토리 세이브 데이터가 없다면, 불러오기 비활성화
	if (!bHasSaveGameData)
	{
		MainScreenWidget->DisableLoadGameButton();
	}
}


void UEntireMainScreen::TryStartNewGame()
{
	// 세이브 데이터가 있었다면 새 게임 덮어쓰기 화면으로 이동
	if (bHasSaveGameData)
	{
		SwitchToOverrideNewGameWidget();
	}
	else
	{
		// 튜토리얼 시작 (기능 재활용)
		UE_LOG(LogMainScreen, Log, TEXT("EntireMainScreen: 새 게임 시작하기 위해 OverrideNewGame의 기능 사용"));
		MainScreenOverrideNewGameWidget->OnYesButtonClicked();
	}
}


void UEntireMainScreen::SwitchToMainScreenWidget()
{
	MainScreenWidgetSwitcher->SetActiveWidget(MainScreenWidget);
	MainScreenWidget->OnSwitchedToThis();
	phase = EMainScreenWidgetPhase::Main;

	ToggleMainBackgroundButton->SetVisibility(bHasBeenClearedOnce ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	UE_LOG(LogMainScreen, Log, TEXT("EntireMainScreen: 메인 화면으로 이동"));
}


void UEntireMainScreen::SwitchToOverrideNewGameWidget()
{
	MainScreenWidgetSwitcher->SetActiveWidget(MainScreenOverrideNewGameWidget);
	MainScreenOverrideNewGameWidget->OnSwitchedToThis();
	phase = EMainScreenWidgetPhase::OverrideNewGame;

	ToggleMainBackgroundButton->SetVisibility(ESlateVisibility::Collapsed);
	UE_LOG(LogMainScreen, Log, TEXT("EntireMainScreen: 새 게임 덮어쓰기 화면으로 이동"));
}


void UEntireMainScreen::SwitchToOptionWidget()
{
	MainScreenWidgetSwitcher->SetActiveWidget(MainScreenOptionWidget);
	MainScreenOptionWidget->OnSwitchedToThis();
	phase = EMainScreenWidgetPhase::Option;

	ToggleMainBackgroundButton->SetVisibility(ESlateVisibility::Collapsed);
	UE_LOG(LogMainScreen, Log, TEXT("EntireMainScreen: 옵션 화면으로 이동"));
}


void UEntireMainScreen::SwitchToCreditWidget()
{
	MainScreenWidgetSwitcher->SetActiveWidget(MainScreenCreditWidget);
	MainScreenCreditWidget->OnSwitchedToThis();
	phase = EMainScreenWidgetPhase::Credit;

	ToggleMainBackgroundButton->SetVisibility(ESlateVisibility::Collapsed);
	UE_LOG(LogMainScreen, Log, TEXT("EntireMainScreen: 크레딧 화면으로 이동"));
}
