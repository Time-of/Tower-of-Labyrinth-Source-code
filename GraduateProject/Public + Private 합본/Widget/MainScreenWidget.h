
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget/CanConsumeAxisInputWidgetBase.h"
#include "MainScreenWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewGameButtonClickedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOptionButtonClickedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreditButtonClickedDelegate);


/**
 * @author 이성수
 * @brief 게임의 메인 스크린 위젯입니다. EntireMainScreen에 속해 있습니다.
 * @since 2023-11-10
 */
UCLASS()
class GRADUATEPROJECT_API UMainScreenWidget : public UCanConsumeAxisInputWidgetBase
{
	GENERATED_BODY()
	
public:
	UMainScreenWidget(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() final;
	void NativeDestruct() final;

	/** @brief 선택에 대한 키보드 입력 지원을 위해 사용 */
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) final;

	void OnSwitchedToThis();

	void DisableLoadGameButton();


	FOnNewGameButtonClickedDelegate OnNewGameButtonClickedDelegate;
	FOnOptionButtonClickedDelegate OnOptionButtonClickedDelegate;
	FOnCreditButtonClickedDelegate OnCreditButtonClickedDelegate;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "TOL|MainScreen|MainScreenWidget")
	FName townLevelName = TEXT("tawn");


protected:
	void SelectUpperMenu();
	void SelectUnderMenu();

	UFUNCTION(BlueprintCallable, Category = "TOL|MainScreen|MainScreenWidget")
	void SelectMenu(int Index);
	void UnselectMenu();


protected:
	UFUNCTION()
	void OnNewGameButtonClicked();
	UFUNCTION()
	void OnLoadGameButtonClicked();
	UFUNCTION()
	void OnOptionButtonClicked();
	UFUNCTION()
	void OnCreditButtonClicked();
	UFUNCTION()
	void OnExitButtonClicked();

	UFUNCTION()
	void OnNewGameButtonHovered() { SelectMenu(0); }
	UFUNCTION()
	void OnLoadGameButtonHovered() { SelectMenu(1); }
	UFUNCTION()
	void OnOptionButtonHovered() { SelectMenu(2); }
	UFUNCTION()
	void OnCreditButtonHovered() { SelectMenu(3); }
	UFUNCTION()
	void OnExitButtonHovered() { SelectMenu(4); }


protected:
	UPROPERTY(Transient)
	int8 nowSelectedButtonIndex = 0;


protected:
	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|MainScreenWidget", meta = (BindWidget))
	TObjectPtr<class UVerticalBox> ButtonsVerticalBox = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|MainScreenWidget", meta = (BindWidget))
	TObjectPtr<class UButton> NewGameButton = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|MainScreenWidget", meta = (BindWidget))
	TObjectPtr<class UButton> LoadGameButton = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|MainScreenWidget", meta = (BindWidget))
	TObjectPtr<class UButton> OptionButton = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|MainScreenWidget", meta = (BindWidget))
	TObjectPtr<class UButton> CreditButton = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|MainScreenWidget", meta = (BindWidget))
	TObjectPtr<class UButton> ExitButton = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|MainScreenWidget", meta = (BindWidget))
	TObjectPtr<class USizeBox> NowSelectSizeBox = nullptr;


protected:
	// UCanConsumeAxisInputWidgetBase을(를) 통해 상속됨
	virtual void OnAxisPositive() override;
	virtual void OnAxisNegative() override;
};
