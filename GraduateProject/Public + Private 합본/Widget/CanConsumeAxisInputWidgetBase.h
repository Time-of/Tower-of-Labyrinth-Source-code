
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CanConsumeAxisInputWidgetBase.generated.h"

/**
 * @author 이성수
 * @brief 축 기반 입력을 받을 수 있는 위젯의 베이스 클래스입니다.
 * @since 2023-11-11
 */
UCLASS()
class GRADUATEPROJECT_API UCanConsumeAxisInputWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	* @brief 축 입력을 시도합니다.
	* @details NativeOnKeyUp 또는 NativeOnKeyDown에서 사용할 것!
	* @return 성공 시 FReply::Handled(), 실패 시 FReply::Unhandled()를 반환합니다.
	*/
	virtual FReply TryConsumeAxisInput(const FKeyEvent& InKeyEvent, const TArray<FInputAxisKeyMapping>& AxisMappings);
	
	virtual void OnAxisPositive() {}
	virtual void OnAxisNegative() {}


protected:
	/** @brief 입력 축의 임계값(이 값 이상일 때 체크) */
	UPROPERTY(EditDefaultsOnly, Category = "TOL|AxisInput")
	float selectInputThreshold = 0.3f;
};
