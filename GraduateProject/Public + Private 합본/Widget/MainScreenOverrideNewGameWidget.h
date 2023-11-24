
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget/CanConsumeAxisInputWidgetBase.h"
#include "MainScreenOverrideNewGameWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoButtonClickedDelegate);

/**
 * @author 이성수
 * @brief 메인 스크린에서 새 게임 오버라이드를 담당하는 위젯입니다.
 * @since 2023-11-11
 */
UCLASS()
class GRADUATEPROJECT_API UMainScreenOverrideNewGameWidget : public UCanConsumeAxisInputWidgetBase
{
	GENERATED_BODY()
	
public:
	UMainScreenOverrideNewGameWidget(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() final;
	void NativeDestruct() final;

	/** @brief 선택에 대한 키보드 입력 지원을 위해 사용 */
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) final;

	void OnSwitchedToThis();

	FOnNoButtonClickedDelegate OnNoButtonClickedDelegate;


	void SelectYes();
	void SelectNo();

	UFUNCTION()
	void OnYesButtonClicked();

	UFUNCTION()
	void OnNoButtonClicked();


protected:
	UPROPERTY()
	bool bNowYesSelected = false;

	UPROPERTY(EditDefaultsOnly, Category = "TOL|MainScreen|OverrideNewGame")
	FName tutorialLevelName = TEXT("Player_Tutorial");


	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|OverrideNewGame", meta = (BindWidget))
	TObjectPtr<class UButton> YesButton = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|OverrideNewGame", meta = (BindWidget))
	TObjectPtr<class UButton> NoButton = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|OverrideNewGame", meta = (BindWidget))
	TObjectPtr<class USizeBox> YesSelected = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|OverrideNewGame", meta = (BindWidget))
	TObjectPtr<class USizeBox> NoSelected = nullptr;


protected:
	// UCanConsumeAxisInputWidgetBase을(를) 통해 상속됨
	virtual void OnAxisPositive() override;
	virtual void OnAxisNegative() override;
};
