
#pragma once

#include "CoreMinimal.h"
#include "Widget/Option/OptionBlockBase.h"
#include "OptionBlock_Toggle.generated.h"

/**
 * @author 이성수
 * @brief 토글 버튼을 가지고 있는 옵션 블록 위젯 클래스입니다.
 * @since 2023-11-11
 */
UCLASS()
class GRADUATEPROJECT_API UOptionBlock_Toggle : public UOptionBlockBase
{
	GENERATED_BODY()
	
public:
	/**
	* @brief On/Off(bIsActivated)를 토글합니다.
	* @return 토글 후 결과를 반환합니다.
	*/
	bool ToggleButton();

	/**
	* @brief 현재 상태 설정
	* @param bIsOn true라면 On, false라면 Off
	*/
	void SetCurrentState(bool bIsOn);

	bool GetCurrentState() const { return bIsActivated; }


public:
	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionBlock", meta = (BindWidget))
	TObjectPtr<class UButton> SettingToggleButton = nullptr;


protected:
	/** @brief 현재 상태 텍스트. On / Off */
	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionBlock", meta = (BindWidget))
	TObjectPtr<class UTextBlock> CurrentStateText = nullptr;

	UPROPERTY()
	bool bIsActivated = false;
};
