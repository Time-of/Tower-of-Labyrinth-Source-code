
#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Abilities/Attributes/AttributeSetBase.h"
#include "GEC_DamageExecution.generated.h"



struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageToDirectHealth);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Guard);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageMultOnGuardExists);
	DECLARE_ATTRIBUTE_CAPTUREDEF(GuardDamageTakenMult);

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Damage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Strength, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, DamageToDirectHealth, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Guard, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, DamageMultOnGuardExists, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, GuardDamageTakenMult, Target, false);
	}
};



static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DamageStatics;

	return DamageStatics;
}



/**
 * @author 이성수
 * @brief 피해량을 계산해주는 EffectExecutionCalculation 클래스
 * @details 주로 GameplayEffect에서 Calculation Class로써 사용됩니다.
 * @see Execute_Implementation() 참고.
 * @since 2023-01-31
 */
UCLASS()
class GRADUATEPROJECT_API UGEC_DamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	

public:
	UGEC_DamageExecution();

	/**
	* @brief 이 곳에서 실제 피해량 계산을 수행합니다.
	* @see UGEC_DamageExecution 참고.
	*/
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	virtual void ApplyDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float DamageAmount) const;

	virtual void ApplyGuardDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float DamageAmount) const;
};
