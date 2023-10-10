
#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEC_DirectDamageToGuard.generated.h"



/**
 * @author 이성수
 * @brief 가드 게이지에 직접적으로 피해를 주는 계산 클래스입니다.
 * @since 2023-04-20
 */
UCLASS()
class GRADUATEPROJECT_API UGEC_DirectDamageToGuard : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UGEC_DirectDamageToGuard();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
