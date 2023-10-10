
#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEC_MaxHealthProportionalDamage.generated.h"



/**
 * @author 이성수
 * @brief 최대 체력 비례 피해를 주는 EffectExecutionCalculation 클래스
 * @details 주로 GameplayEffect에서 Calculation Class로써 사용됩니다.
 * @details **피해량 공식은 기본적으로 (Effect의 Damage * 타겟의 MaxHealth)**
 * @details 즉, 상술한 Damage가 0.01이라면 대상 최대 체력의 1% 피해를 입히게 됩니다.
 * @details Data.DamageMult의 영향을 받지 않습니다.
 * @details **대상의 OnDamaged를 발생시키지 않습니다. 즉, OnHealthChanged만 발생시킵니다.**
 * @details 기획자 요청으로, Guard 수치에 무관하게 피해를 입힙니다. (즉, Guard 수치는 무시됨.)
 * @see Execute_Implementation() 참고.
 * @since 2023-02-08
 */
UCLASS()
class GRADUATEPROJECT_API UGEC_MaxHealthProportionalDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UGEC_MaxHealthProportionalDamage();

	/**
	* @brief 이 곳에서 실제 피해량 계산을 수행합니다.
	* @details **피해량 공식은 기본적으로 (Effect의 Damage * 타겟의 MaxHealth)**
	*/
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
