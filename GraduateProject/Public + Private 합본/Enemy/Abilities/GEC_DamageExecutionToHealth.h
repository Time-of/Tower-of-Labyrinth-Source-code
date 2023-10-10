
#pragma once

#include "CoreMinimal.h"
#include "Enemy/Abilities/GEC_DamageExecution.h"
#include "GEC_DamageExecutionToHealth.generated.h"



/**
 * @author 이성수
 * @brief 체력에 직접 피해를 주게 하는 피해 계산 클래스입니다.
 * @since 2023-05-19
 */
UCLASS()
class GRADUATEPROJECT_API UGEC_DamageExecutionToHealth : public UGEC_DamageExecution
{
	GENERATED_BODY()

public:
	UGEC_DamageExecutionToHealth();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	void ApplyDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float DamageAmount) const final;
};
