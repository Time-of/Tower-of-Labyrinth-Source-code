
#include "Enemy/Abilities/GEC_DirectDamageToGuard.h"

#include "CharacterBase.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Abilities/Attributes/AttributeSetBase.h"



struct FGuardDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Guard);
	DECLARE_ATTRIBUTE_CAPTUREDEF(InternalMaxGuard);
	DECLARE_ATTRIBUTE_CAPTUREDEF(GuardDamageTakenMult);

	FGuardDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Damage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Guard, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, InternalMaxGuard, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, GuardDamageTakenMult, Target, false);
	}
};



static const FGuardDamageStatics& GuardDamageStatics()
{
	static FGuardDamageStatics _GuardDamageStatics;

	return _GuardDamageStatics;
}



UGEC_DirectDamageToGuard::UGEC_DirectDamageToGuard()
{
	RelevantAttributesToCapture.Add(GuardDamageStatics().DamageDef);

	RelevantAttributesToCapture.Add(GuardDamageStatics().GuardDef);
	RelevantAttributesToCapture.Add(GuardDamageStatics().InternalMaxGuardDef);
	RelevantAttributesToCapture.Add(GuardDamageStatics().GuardDamageTakenMultDef);
}



void UGEC_DirectDamageToGuard::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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



	float InternalMaxGuard = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GuardDamageStatics().InternalMaxGuardDef, EvaluationParameters, InternalMaxGuard);

	float GuardDamageTakenMult = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GuardDamageStatics().GuardDamageTakenMultDef, EvaluationParameters, GuardDamageTakenMult);

	// 여기서 피해량 계산 (곱연산, 1.0일 때 최대 가드만큼 가드에 피해)
	InternalMaxGuard *= FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(
		TEXT("Data.DamageMult"), false)), 0.0f);

	// Data.DirectDamageToGuard.UseGuardDamageTakenMult가 0.01f보다 크면 true
	bool bUseGuardDamageTakenMult = (Spec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(TEXT("Data.DirectDamageToGuard.UseGuardDamageTakenMult"), false),
		1.0f)) > 0.01f;

	float CalculatedDamage = bUseGuardDamageTakenMult ? (InternalMaxGuard * GuardDamageTakenMult) : InternalMaxGuard;



	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GuardDamageStatics().GuardProperty,
		EGameplayModOp::Additive, -CalculatedDamage));
}
