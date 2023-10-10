
#include "Enemy/Abilities/GameplayAbilityBase.h"

#include "BlueprintFunctionLibrary/GameplayHelperLibrary.h"



UGameplayAbilityBase::UGameplayAbilityBase() : Super()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("State.Debuff.Stun")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability")));

	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability")));
}



void UGameplayAbilityBase::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	OnInputButtonPressed(Handle, ActivationInfo);
}



void UGameplayAbilityBase::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	OnInputButtonReleased(Handle, ActivationInfo);
}

