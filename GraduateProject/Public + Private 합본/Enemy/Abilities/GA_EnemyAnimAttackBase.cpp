
#include "Enemy/Abilities/GA_EnemyAnimAttackBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "CharacterBase.h"
#include "Enemy/Interface/EnemyHandler.h"
#include <GraduateProject/GraduateProject.h>


UGA_EnemyAnimAttackBase::UGA_EnemyAnimAttackBase() : 
	AttackAnimMontage(nullptr),
	LockAbilityEffectDuration(1.5f)
{
	EnemyAbilityHitTag = FGameplayTag::RequestGameplayTag(TEXT("Event.Enemy.Ability.Hit"));
	DamageDataTag = FGameplayTag::RequestGameplayTag(TEXT("Data.DamageMult"));
	LockAbilityDataTag = FGameplayTag::RequestGameplayTag(TEXT("Data.LockAbilityDuration"));
}


void UGA_EnemyAnimAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());

	if (!IsValid(Character))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		UE_LOG(LogAnimAttackAbility, Warning, TEXT("캐릭터 유효하지 않음"));
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontagePlayTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AttackAnimMontage,
		Character->GetAttackSpeed() > 0.0f ? Character->GetAttackSpeed() : 1.0f,
		NAME_None
	);

	MontagePlayTask->OnCompleted.AddDynamic(this, &UGA_EnemyAnimAttackBase::OnCompleted);
	MontagePlayTask->OnBlendOut.AddDynamic(this, &UGA_EnemyAnimAttackBase::OnCompleted);
	MontagePlayTask->OnCancelled.AddDynamic(this, &UGA_EnemyAnimAttackBase::OnCancelled);
	MontagePlayTask->OnInterrupted.AddDynamic(this, &UGA_EnemyAnimAttackBase::OnCancelled);
	MontagePlayTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		EnemyAbilityHitTag
	);

	WaitEventTask->EventReceived.AddDynamic(this, &UGA_EnemyAnimAttackBase::EventReceived);
	WaitEventTask->ReadyForActivation();
}


void UGA_EnemyAnimAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetAvatarActorFromActorInfo()->CustomTimeDilation = 1.0f;

	if (LockAbilityEffectDuration <= 0.0f) return;

	auto SpecHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		MakeOutgoingGameplayEffectSpec(LockAbilityEffectClass),
		LockAbilityDataTag,
		LockAbilityEffectDuration
	);

	K2_ApplyGameplayEffectSpecToOwner(SpecHandle);
}


void UGA_EnemyAnimAttackBase::EventReceived(FGameplayEventData EventData)
{
	for (auto& EffectClass : EffectClassesAppliedOnTarget)
	{
		ApplyEffectClassToTarget(EffectClass, EventData, DamageDataTag);
	}


	if (GetAvatarActorFromActorInfo()->Implements<UEnemyHandler>())
	{
		TSubclassOf<UGameplayEffect> Additional = IEnemyHandler::Execute_GetAdditionalAttackEffectOnTarget(GetAvatarActorFromActorInfo());

		if (Additional != nullptr)
		{
			ApplyEffectClassToTarget(Additional, EventData, DamageDataTag);
		}
	}

	K_EventReceived(EventData);
}


void UGA_EnemyAnimAttackBase::ApplyEffectClassToTarget(TSubclassOf<UGameplayEffect> EffectClass, FGameplayEventData& EventData, FGameplayTag& DataTag)
{
	auto SpecHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		MakeOutgoingGameplayEffectSpec(EffectClass),
		DataTag,
		EventData.EventMagnitude
	);

	K2_ApplyGameplayEffectSpecToTarget(SpecHandle, EventData.TargetData);
}
