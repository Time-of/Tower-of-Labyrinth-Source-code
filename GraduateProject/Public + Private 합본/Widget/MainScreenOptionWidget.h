
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainScreenOptionWidget.generated.h"

/**
 * @author 이성수
 * @brief 메인 스크린에서 설정을 담당하는 위젯입니다. OptionPage 들을 담고 있습니다.
 * @details 조작, 그래픽, 오디오, 기타 설정 가능 (그 외 메뉴가 추가될 예정 있음)
 * @since 2023-11-11
 */
UCLASS()
class GRADUATEPROJECT_API UMainScreenOptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainScreenOptionWidget(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() final;
	void NativeDestruct() final;

	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) final;

	void OnSwitchedToThis();


protected:
	void SetButtonKeyTexts();


protected:
	UFUNCTION()
	void SwitchToControlsPanel();

	UFUNCTION()
	void SwitchToGraphicsPanel();

	UFUNCTION()
	void OnSaveChangesButtonClicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "TOL|MainScreen|Option")
	void PostSaveChangedsButtonClicked();

	UFUNCTION()
	void OnControlPanelButtonHovered();

	UFUNCTION()
	void OnGraphicsPanelButtonHovered();


private:
	void _OnPanelButtonHovered(TObjectPtr<class UPanelButton> button);

	void _OnPanelButtonSelected(TObjectPtr<class UPanelButton> button);



protected:
	/** @brief 저장 버튼 사용 가능 여부. 블루프린트에서 관리 */
	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|Option")
	bool bCanSaveChanges = true;

	UPROPERTY()
	TArray<FInputActionKeyMapping> InteractMapping;
	UPROPERTY()
	TArray<FInputActionKeyMapping> EscapeMapping;


#pragma region 패널 버튼
	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|Option", meta = (BindWidget))
	TObjectPtr<class UPanelButton> PanelButton_Controls = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|Option", meta = (BindWidget))
	TObjectPtr<class UPanelButton> PanelButton_Graphics = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|Option", meta = (BindWidget))
	TObjectPtr<class UPanelButton> PanelButton_Audio = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|Option", meta = (BindWidget))
	TObjectPtr<class UPanelButton> PanelButton_Language = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|Option", meta = (BindWidget))
	TObjectPtr<class UPanelButton> PanelButton_Keys = nullptr;

	UPROPERTY()
	TObjectPtr<class UPanelButton> PanelButton_CurrentHovered = nullptr;

	UPROPERTY()
	TObjectPtr<class UPanelButton> PanelButton_CurrentClicked = nullptr;
#pragma endregion 패널 버튼


	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|Option", meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> OptionPageSwitcher = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|Option", meta = (BindWidget))
	TObjectPtr<class UOptionPage_Controls> OptionPage_Controls = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|Option", meta = (BindWidget))
	TObjectPtr<class UOptionPage_Graphics> OptionPage_Graphics = nullptr;


	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|Option", meta = (BindWidget))
	TObjectPtr<class UButton> SaveChangesButton = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|Option", meta = (BindWidget))
	TObjectPtr<class UTextBlock> SaveButtonKeyText = nullptr;


public:
	/** @details EntireMainScreen에서 직접 바인딩 */
	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|Option", meta = (BindWidget))
	TObjectPtr<class UButton> ReturnToMainScreenButton = nullptr;


protected:
	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|Option", meta = (BindWidget))
	TObjectPtr<class UTextBlock> ReturnButtonKeyText = nullptr;
};
