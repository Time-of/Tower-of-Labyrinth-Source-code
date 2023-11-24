
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EntireMainScreen.generated.h"


UENUM(BlueprintType)
enum class EMainScreenWidgetPhase : uint8
{
	Main, OverrideNewGame, Option, Credit
};


/**
 * @author 이성수
 * @brief 게임의 전체 메인 스크린 위젯입니다.
 * @details MainScreenWidget 등을 여기에서 기능별로 관리합니다.
 * @since 2023-11-10
 */
UCLASS()
class GRADUATEPROJECT_API UEntireMainScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	UEntireMainScreen(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() final;
	void NativeDestruct() final;


protected:
	void CheckClearInfo();
	void CheckHasSaveGameData();

	UFUNCTION(BlueprintImplementableEvent, Category = "TOL|MainScreen|EntireMainScreen")
	void OnClearInfoExists();


	UFUNCTION()
	void TryStartNewGame();

	UFUNCTION()
	void SwitchToMainScreenWidget();

	UFUNCTION()
	void SwitchToOverrideNewGameWidget();

	UFUNCTION()
	void SwitchToOptionWidget();

	UFUNCTION()
	void SwitchToCreditWidget();


protected:
	UPROPERTY()
	EMainScreenWidgetPhase phase = EMainScreenWidgetPhase::Main;

	/** @brief 한 번이라도 클리어 한 적이 있는가? */
	UPROPERTY()
	bool bHasBeenClearedOnce = false;

	/** @brief 세이브 게임 데이터가 있는가? */
	UPROPERTY()
	bool bHasSaveGameData = false;


protected:
	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|EntireMainScreen", meta = (BindWidget))
	TObjectPtr<class UImage> MainBackgroundImage = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|EntireMainScreen", meta = (BindWidget))
	TObjectPtr<class UButton> ToggleMainBackgroundButton = nullptr;


#pragma region 메인스크린 자식 위젯들 관리
	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|EntireMainScreen", meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> MainScreenWidgetSwitcher = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|EntireMainScreen", meta = (BindWidget))
	TObjectPtr<class UMainScreenWidget> MainScreenWidget = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|EntireMainScreen", meta = (BindWidget))
	TObjectPtr<class UMainScreenOverrideNewGameWidget> MainScreenOverrideNewGameWidget = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|EntireMainScreen", meta = (BindWidget))
	TObjectPtr<class UMainScreenOptionWidget> MainScreenOptionWidget = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|EntireMainScreen", meta = (BindWidget))
	TObjectPtr<class UMainScreenCreditWidget> MainScreenCreditWidget = nullptr;
#pragma endregion 메인스크린 자식 위젯들 관리
};
