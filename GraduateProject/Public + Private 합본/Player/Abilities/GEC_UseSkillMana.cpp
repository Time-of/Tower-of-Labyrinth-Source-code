
#include "Player/Abilities/GEC_UseSkillMana.h"

#include "Player/Abilities/Attributes/PlayerAttributeSet.h"

#include "GameplayEffectExecutionCalculation.h"



struct FPlayerSkillManaCalculationStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(SkillMana);
	DECLARE_ATTRIBUTE_CAPTUREDEF(SkillManaUsageRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(SkillManaAmountToUse);

	FPlayerSkillManaCalculationStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, SkillMana, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, SkillManaUsageRate, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, SkillManaAmountToUse, Source, true);
	}
};



static const FPlayerSkillManaCalculationStatics& SkillManaStatics()
{
	static FPlayerSkillManaCalculationStatics SkillManaStatics;

	return SkillManaStatics;
}



UGEC_UseSkillMana::UGEC_UseSkillMana()
{
	//FGameplayEffectAttributeCaptureDefinition SkillManaCaptureDefinition;
	//SkillManaCaptureDefinition.AttributeToCapture = UPlayerAttributeSet::GetSkillManaAttribute();
	//SkillManaCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	//SkillManaCaptureDefinition.bSnapshot = false;

	//RelevantAttributesToCapture.Add(SkillManaCaptureDefinition);

	//FGameplayEffectAttributeCaptureDefinition SkillManaUsageRateCaptureDefinition;
	//SkillManaUsageRateCaptureDefinition.AttributeToCapture = UPlayerAttributeSet::GetSkillManaUsageRateAttribute();
	//SkillManaUsageRateCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	//SkillManaUsageRateCaptureDefinition.bSnapshot = false;

	//RelevantAttributesToCapture.Add(SkillManaUsageRateCaptureDefinition);
	
	RelevantAttributesToCapture.Add(SkillManaStatics().SkillManaDef);
	RelevantAttributesToCapture.Add(SkillManaStatics().SkillManaUsageRateDef);
}



float UGEC_UseSkillMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;

	const UAbilitySystemComponent* SourceASC = Spec.GetContext().GetInstigatorAbilitySystemComponent();
	const UPlayerAttributeSet* PlayerAttributeSet = SourceASC->GetSet<UPlayerAttributeSet>();
	
	if (IsValid(PlayerAttributeSet))
	{
		float SkillManaCost = 1.0f;

		float SkillManaUsageRate = PlayerAttributeSet->GetSkillManaUsageRate();

		//UE_LOG(LogTemp, Warning, TEXT("Calc: %f, Cost: %f, UsageRate: %f"), SkillManaCost * SkillManaUsageRate, SkillManaCost, SkillManaUsageRate);

		return SkillManaCost * SkillManaUsageRate;
	}

	UE_LOG(LogTemp, Warning, TEXT("PlayerAttributeSet 찾지 못함!!"));

	return 0.0f;
}
