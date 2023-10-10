
#include "Enemy/Abilities/GEC_DamageExecution.h"



UGEC_DamageExecution::UGEC_DamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().StrengthDef);

	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().GuardDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageMultOnGuardExistsDef);
	RelevantAttributesToCapture.Add(DamageStatics().GuardDamageTakenMultDef);
}



void UGEC_DamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;



	float DefaultDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, DefaultDamage);

	DefaultDamage *= FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(
		TEXT("Data.DamageMult"), false)), 0.0f);

	float Strength = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().StrengthDef, EvaluationParameters, Strength);

	float Defense = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseDef, EvaluationParameters, Defense);

	float Guard = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().GuardDef, EvaluationParameters, Guard);

	float DamageMultOnGuardExists = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageMultOnGuardExistsDef, EvaluationParameters, DamageMultOnGuardExists);

	float GuardDamageTakenMult = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().GuardDamageTakenMultDef, EvaluationParameters, GuardDamageTakenMult);

	// 최종 피해 배율, 설정하지 않은 경우 기본값 1.0f
	float FinalDamageRate = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(
		TEXT("Data.FinalDamageRate"), false), false, 1.0f), 0.0f);

	// 계산된 타겟의 방어력 수치에 따른 받는 피해량 수치 (최대 30%까지만 적용됨)
	float CalculatedTargetDamageAppliedRateByDefenseAmount = FMath::Max(1 - (Defense / (2500.0f + Defense)), 0.7f);

	float CalculatedDamage = (DefaultDamage + Strength) * CalculatedTargetDamageAppliedRateByDefenseAmount * (Guard > 0.0f ? DamageMultOnGuardExists : 1.0f) * FinalDamageRate;

	if (CalculatedDamage < 0.0f) CalculatedDamage = 0.0f;
	



	ApplyDamage(OutExecutionOutput, CalculatedDamage);

	ApplyGuardDamage(OutExecutionOutput, -(DefaultDamage + Strength) * GuardDamageTakenMult);
}



void UGEC_DamageExecution::ApplyDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float DamageAmount) const
{
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty,
		EGameplayModOp::Additive, DamageAmount));
}



void UGEC_DamageExecution::ApplyGuardDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float DamageAmount) const
{
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().GuardProperty,
		EGameplayModOp::Additive, DamageAmount));
}
