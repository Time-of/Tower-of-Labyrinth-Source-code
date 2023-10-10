
#include "Player/Abilities/Attributes/PlayerAttributeSet.h"

#include "Player/PlayerCharacterBase.h"

#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"



void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	UAttributeSetBase::PostGameplayEffectExecute(Data);

	float DeltaValue = 0.0f;

	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive)
	{
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	FGameplayEffectContextHandle EffectContext = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* SourceASC = EffectContext.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	APlayerCharacterBase* TargetCharacter = nullptr;

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<APlayerCharacterBase>(TargetActor);
	}

	// 스킬 게이지(SkillMana)에 관한 처리
	if (Data.EvaluatedData.Attribute == GetSkillManaAttribute())
	{
		if (TargetCharacter != nullptr)
		{
			SetSkillMana(FMath::Clamp(GetSkillMana(), 0.0f, GetMaxSkillMana()));

			TargetCharacter->OnSkillManaChanged(DeltaValue, SourceTags);
		}
	}
	// 최대 스킬 게이지에 관한 처리
	else if (Data.EvaluatedData.Attribute == GetMaxSkillManaAttribute())
	{
		if (TargetCharacter != nullptr)
		{
			TargetCharacter->OnMaxSkillManaChanged(DeltaValue, SourceTags);
		}
	}
}
