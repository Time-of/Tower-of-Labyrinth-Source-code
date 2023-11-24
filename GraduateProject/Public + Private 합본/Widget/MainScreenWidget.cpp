
#include "Widget/MainScreenWidget.h"

#include "MainScreenOptionWidget.h"
#include "BlueprintFunctionLibrary/DataSaveLoadHelper.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include <GraduateProject/GraduateProject.h>


UMainScreenWidget::UMainScreenWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}


void UMainScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	nowSelectedButtonIndex = 0;
	SelectMenu(0);

	NewGameButton->OnClicked.AddUniqueDynamic(this, &UMainScreenWidget::OnNewGameButtonClicked);
	LoadGameButton->OnClicked.AddUniqueDynamic(this, &UMainScreenWidget::OnLoadGameButtonClicked);
	OptionButton->OnClicked.AddUniqueDynamic(this, &UMainScreenWidget::OnOptionButtonClicked);
	CreditButton->OnClicked.AddUniqueDynamic(this, &UMainScreenWidget::OnCreditButtonClicked);
	ExitButton->OnClicked.AddUniqueDynamic(this, &UMainScreenWidget::OnExitButtonClicked);
	NewGameButton->OnHovered.AddUniqueDynamic(this, &UMainScreenWidget::OnNewGameButtonHovered);
	LoadGameButton->OnHovered.AddUniqueDynamic(this, &UMainScreenWidget::OnLoadGameButtonHovered);
	OptionButton->OnHovered.AddUniqueDynamic(this, &UMainScreenWidget::OnOptionButtonHovered);
	CreditButton->OnHovered.AddUniqueDynamic(this, &UMainScreenWidget::OnCreditButtonHovered);
	ExitButton->OnHovered.AddUniqueDynamic(this, &UMainScreenWidget::OnExitButtonHovered);
}


void UMainScreenWidget::NativeDestruct()
{
	Super::NativeDestruct();

	NewGameButton->OnClicked.RemoveDynamic(this, &UMainScreenWidget::OnNewGameButtonClicked);
	LoadGameButton->OnClicked.RemoveDynamic(this, &UMainScreenWidget::OnLoadGameButtonClicked);
	OptionButton->OnClicked.RemoveDynamic(this, &UMainScreenWidget::OnOptionButtonClicked);
	CreditButton->OnClicked.RemoveDynamic(this, &UMainScreenWidget::OnCreditButtonClicked);
	ExitButton->OnClicked.RemoveDynamic(this, &UMainScreenWidget::OnExitButtonClicked);
	NewGameButton->OnHovered.RemoveDynamic(this, &UMainScreenWidget::OnNewGameButtonHovered);
	LoadGameButton->OnHovered.RemoveDynamic(this, &UMainScreenWidget::OnLoadGameButtonHovered);
	OptionButton->OnHovered.RemoveDynamic(this, &UMainScreenWidget::OnOptionButtonHovered);
	CreditButton->OnHovered.RemoveDynamic(this, &UMainScreenWidget::OnCreditButtonHovered);
	ExitButton->OnHovered.RemoveDynamic(this, &UMainScreenWidget::OnExitButtonHovered);
}


FReply UMainScreenWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	TArray<FInputActionKeyMapping> OutActionMappings;
	TArray<FInputAxisKeyMapping> OutAxisMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName(TEXT("Escape"), OutActionMappings);
	UInputSettings::GetInputSettings()->GetActionMappingByName(TEXT("Interact"), OutActionMappings);
	UInputSettings::GetInputSettings()->GetActionMappingByName(TEXT("UI_Confirm"), OutActionMappings);
	UInputSettings::GetInputSettings()->GetAxisMappingByName(TEXT("MoveForward"), OutAxisMappings);

	check(OutActionMappings.Num() >= 3);
	check(!OutAxisMappings.IsEmpty());


	// Interact나 UI_Confirm 입력을 받은 경우
	if (InKeyEvent.GetKey() == OutActionMappings[1].Key || InKeyEvent.GetKey() == OutActionMappings[2].Key)
	{
		UE_LOG(LogMainScreen, Log, TEXT("메인 스크린: 확인 액션"));

		switch (nowSelectedButtonIndex)
		{
		case 0:
			OnNewGameButtonClicked();
			break;
		case 1:
			OnLoadGameButtonClicked();
			break;
		case 2:
			OnOptionButtonClicked();
			break;
		case 3:
			OnCreditButtonClicked();
			break;
		case 4:
			OnExitButtonClicked();
			break;
		}

		return FReply::Handled();
	}
	// Escape 입력을 받은 경우
	else if (InKeyEvent.GetKey() == OutActionMappings[0].Key)
	{
		UE_LOG(LogMainScreen, Log, TEXT("메인 스크린: 취소 액션"));

		return FReply::Handled();
	}

	return TryConsumeAxisInput(InKeyEvent, OutAxisMappings);
}


void UMainScreenWidget::OnSwitchedToThis()
{
	SelectMenu(nowSelectedButtonIndex);
}


void UMainScreenWidget::DisableLoadGameButton()
{
	LoadGameButton->SetIsEnabled(false);
}


void UMainScreenWidget::SelectUpperMenu()
{
	SelectMenu(nowSelectedButtonIndex - 1);
}


void UMainScreenWidget::SelectUnderMenu()
{
	SelectMenu(nowSelectedButtonIndex + 1);
}


void UMainScreenWidget::SelectMenu(int Index)
{
	UWidget* target = ButtonsVerticalBox->GetChildAt(Index);
	UHorizontalBox* hb = nullptr;

	if (IsValid(target))
	{
		hb = Cast<UHorizontalBox>(target);
		check(hb);

		UnselectMenu();

		hb->AddChildToHorizontalBox(NowSelectSizeBox);
		nowSelectedButtonIndex = Index;
		hb->SetRenderOpacity(1.0f);
		hb->GetChildAt(0)->SetFocus();

		UE_LOG(LogMainScreen, Log, TEXT("메뉴 %d 선택!"), nowSelectedButtonIndex);
	}
	else return;
}


void UMainScreenWidget::UnselectMenu()
{
	ButtonsVerticalBox->GetChildAt(nowSelectedButtonIndex)->SetRenderOpacity(0.5f);
}


void UMainScreenWidget::OnNewGameButtonClicked()
{
	UE_LOG(LogMainScreen, Log, TEXT("메인 스크린: 새 게임 버튼 클릭"));
	OnNewGameButtonClickedDelegate.Broadcast();
}


void UMainScreenWidget::OnLoadGameButtonClicked()
{
	UE_LOG(LogMainScreen, Log, TEXT("메인 스크린: 불러오기 버튼 클릭"));

	if (!LoadGameButton->GetIsEnabled()) return;

	// Load Game
	UE_LOG(LogMainScreen, Log, TEXT("메인 스크린: 게임 불러오기 작업 실행"));
	UDataSaveLoadHelper::LoadGameData(GetOwningPlayer());
	UDataSaveLoadHelper::OpenTownLevel(GetOwningPlayer(), townLevelName);
}


void UMainScreenWidget::OnOptionButtonClicked()
{
	UE_LOG(LogMainScreen, Log, TEXT("메인 스크린: 옵션 버튼 클릭"));
	OnOptionButtonClickedDelegate.Broadcast();
}


void UMainScreenWidget::OnCreditButtonClicked()
{
	UE_LOG(LogMainScreen, Log, TEXT("메인 스크린: 크레딧 버튼 클릭"));
	OnCreditButtonClickedDelegate.Broadcast();
}


void UMainScreenWidget::OnExitButtonClicked()
{
	UE_LOG(LogMainScreen, Log, TEXT("메인 스크린: 나가기 버튼 클릭"));
	UKismetSystemLibrary::QuitGame(GetOwningPlayer(),
		GetOwningPlayer(),
		EQuitPreference::Quit,
		false
		);
}


void UMainScreenWidget::OnAxisPositive()
{
	SelectUpperMenu();
}


void UMainScreenWidget::OnAxisNegative()
{
	SelectUnderMenu();
}
