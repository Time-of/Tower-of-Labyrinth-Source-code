
#pragma once

#include "CoreMinimal.h"
#include "Widget/Option/OptionPage.h"
#include "OptionPage_Graphics.generated.h"

/**
 * @author 이성수
 * @brief 그래픽 설정을 담당하는 위젯입니다.
 * @since 2023-11-12
 */
UCLASS()
class GRADUATEPROJECT_API UOptionPage_Graphics : public UOptionPage
{
	GENERATED_BODY()
	
public:
	UOptionPage_Graphics(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() final;
	void NativeDestruct() final;

	/**
	* @brief 현재 선택에 따라 그래픽 세팅을 최종 적용하고 저장
	* @details '그래픽 품질'만 설정. '디스플레이 설정'은 변경 즉시 적용하는 방식
	*/
	void ApplyGraphicSettings();


protected:
	/** @brief 일부 드롭다운 메뉴는 '낮음'~'울트라'가 아닐 수 있는데, 그 부분들을 설정해주는 함수 */
	void SetComboBoxValues();

	/** @brief GameUserSettings에 저장된 데이터를 불러와 OptionBlock들을 설정 */
	void LoadGraphicSettingsInfo();


protected:
	UFUNCTION()
	void OnScreenModeDropdownChanged(FString OptionName, ESelectInfo::Type SelectInfo);

	UFUNCTION()
	void OnResolutionDropdownChanged(FString OptionName, ESelectInfo::Type SelectInfo);

	UFUNCTION()
	void OnMaxFrameLimitDropdownChanged(FString OptionName, ESelectInfo::Type SelectInfo);

	UFUNCTION()
	void OnVSyncButtonClicked();

	UFUNCTION()
	void OnOverallQualityChanged(FString OptionName, ESelectInfo::Type SelectInfo);


protected:
	UPROPERTY()
	TMap<int, float> frameRateLimitMap;


protected:
#pragma region 디스플레이 설정
	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_DropDown> D_ScreenMode = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_DropDown> D_Resolution = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_DropDown> D_MaxFrameLimit = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_Toggle> T_VSync = nullptr;
#pragma endregion 디스플레이 설정


#pragma region 그래픽 품질
	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_DropDown> D_OverallQuality = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_DropDown> D_GlobalIlluminationQuality = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_DropDown> D_TextureQuality = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_DropDown> D_ShadowQuality = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_DropDown> D_EffectQuality = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_DropDown> D_AntialiasingQuality = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_DropDown> D_PostprocessingQuality = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_DropDown> D_ShadingQuality = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_DropDown> D_ReflectionQuality = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_DropDown> D_FoliageQuality = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionPage", meta = (BindWidget))
	TObjectPtr<class UOptionBlock_DropDown> D_ViewDistanceQuality = nullptr;
#pragma endregion 그래픽 품질
};
