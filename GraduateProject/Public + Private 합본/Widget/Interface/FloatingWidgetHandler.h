
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FloatingWidgetHandler.generated.h"



UINTERFACE(MinimalAPI)
class UFloatingWidgetHandler : public UInterface
{
	GENERATED_BODY()
};



/**
 * @author 이성수
 * @brief 게임 중 떠다니는 위젯들(체력바, 피해량 위젯 등)을 컨트롤하기 위한 인터페이스 클래스입니다.
 * @since 2023-02-07
 */
class GRADUATEPROJECT_API IFloatingWidgetHandler
{
	GENERATED_BODY()

public:
	/**
	 * @brief 위젯의 텍스트를 설정합니다.
	 */ 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "FloatingWidgetHandler")
	void SetWidgetText(const FText& TextToSet);

	/**
	 * @brief 위젯의 프로그레스 바 퍼센트를 설정합니다.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "FloatingWidgetHandler")
	void SetProgressPercent(float NewPercent);

	/** @brief 위젯을 숨깁니다. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "FloatingWidgetHandler")
	void HideWidget();
};
