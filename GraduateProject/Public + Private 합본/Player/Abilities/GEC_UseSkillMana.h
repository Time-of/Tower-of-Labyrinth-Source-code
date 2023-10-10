
#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GEC_UseSkillMana.generated.h"



/**
 * @author 이성수
 * @brief 플레이어의 스킬 마나 사용량을 계산하는 계산 클래스입니다.
 * @since 2023-04-26
 */
UCLASS()
class GRADUATEPROJECT_API UGEC_UseSkillMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UGEC_UseSkillMana();

	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
