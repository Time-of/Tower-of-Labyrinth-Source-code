
#include "Enemy/Abilities/GEC_MaxHealthProportionalDamage.h"

#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Abilities/Attributes/AttributeSetBase.h"



struct FProportianalDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	FProportianalDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Damage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, MaxHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Health, Target, false);
	}
};



static const FProportianalDamageStatics& ProportianalDamageStatics()
{
	static FProportianalDamageStatics ProportianalDamageStatics;

	return ProportianalDamageStatics;
}



UGEC_MaxHealthProportionalDamage::UGEC_MaxHealthProportionalDamage()
{
	RelevantAttributesToCapture.Add(ProportianalDamageStatics().DamageDef);

	RelevantAttributesToCapture.Add(ProportianalDamageStatics().MaxHealthDef);
	RelevantAttributesToCapture.Add(ProportianalDamageStatics().HealthDef);
}



void UGEC_MaxHealthProportionalDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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



	float ProportianalDamageMult = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ProportianalDamageStatics().DamageDef, EvaluationParameters, ProportianalDamageMult);

	float TargetMaxHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ProportianalDamageStatics().MaxHealthDef, EvaluationParameters, TargetMaxHealth);

	float CalculatedDamage = TargetMaxHealth * ProportianalDamageMult;



	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(ProportianalDamageStatics().HealthProperty,
		EGameplayModOp::Additive, -CalculatedDamage));
}

