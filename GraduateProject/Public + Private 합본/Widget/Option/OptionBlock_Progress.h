
#pragma once

#include "CoreMinimal.h"
#include "Widget/Option/OptionBlockBase.h"
#include "OptionBlock_Progress.generated.h"

/**
 * @author 이성수
 * @brief 프로그레스 바를 가지고 있는 옵션 블록 위젯 클래스입니다.
 * @since 2023-11-11
 */
UCLASS()
class GRADUATEPROJECT_API UOptionBlock_Progress : public UOptionBlockBase
{
	GENERATED_BODY()
	
public:
	UOptionBlock_Progress(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() final;
	void NativeDestruct() final;

	UFUNCTION(BlueprintCallable, Category = "TOL|OptionWidget|OptionBlock")
	void OnSliderValueSet(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "TOL|OptionWidget|OptionBlock")
	float GetSliderPercent() const;


public:
	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionBlock", meta = (BindWidget))
	TObjectPtr<class UTextBlock> SliderValueText = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionBlock", meta = (BindWidget))
	TObjectPtr<class USlider> SettingSlider = nullptr;


protected:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> SliderMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "TOL|OptionWidget|OptionBlock")
	TObjectPtr<UMaterialInterface> SliderMaterialAsset = nullptr;
};
