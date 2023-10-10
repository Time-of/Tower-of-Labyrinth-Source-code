
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BossHealthbarWidgetHandler.generated.h"



UINTERFACE(MinimalAPI)
class UBossHealthbarWidgetHandler : public UInterface
{
	GENERATED_BODY()
};



/**
 * @author 이성수
 * @brief 보스 체력바 위젯을 컨트롤하기 위한 인터페이스 클래스입니다.
 * @since 2023-02-08
 */
class GRADUATEPROJECT_API IBossHealthbarWidgetHandler
{
	GENERATED_BODY()

public:
	/**
	 * @brief 보스 이름 텍스트를 설정합니다.
	 * 
	 * @param BossNameString 설정할 이름
	 */ 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "BossHealthbarWidgetHandler")
	void SetBossNameText(const FString& BossNameString);

	/**
	 * @brief 체력 퍼센트를 설정합니다.
	 */ 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "BossHealthbarWidgetHandler")
	void SetHealthPercent(float NewPercent);

	/**
	 * @brief 가드 퍼센트를 설정합니다.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "BossHealthbarWidgetHandler")
	void SetGuardPercent(float NewPercent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "BossHealthbarWidgetHandler")
	void StartGuardChargingAnimation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "BossHealthbarWidgetHandler")
	void EndGuardChargingAnimation();
};
