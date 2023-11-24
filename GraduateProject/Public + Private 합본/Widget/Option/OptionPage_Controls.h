
#pragma once

#include "CoreMinimal.h"
#include "Widget/Option/OptionPage.h"
#include "OptionPage_Controls.generated.h"


/**
 * @author 이성수
 * @brief 옵션 페이지 중, 캐릭터 조작을 담당하는 옵션 페이지 위젯입니다.
 * @since 2023-11-14
 */
UCLASS()
class GRADUATEPROJECT_API UOptionPage_Controls : public UOptionPage
{
	GENERATED_BODY()

public:
	void NativeConstruct() final;
	void NativeDestruct() final;

	void ApplyAndSaveSettings();


protected:
	void LoadSettings();


protected:
/*
	UFUNCTION()
	void OnCamSenXSliderValueChanged(float newValue);

	UFUNCTION()
	void OnCamSenYSliderValueChanged(float newValue);
*/
	UFUNCTION()
	void OnInvMouseXButtonClicked();

	UFUNCTION()
	void OnInvMouseYButtonClicked();

	
protected:
	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_Progress> P_CameraSensitivityX = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_Progress> P_CameraSensitivityY = nullptr;


	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_Toggle> T_InvertMouseX = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_Toggle> T_InvertMouseY = nullptr;
};
