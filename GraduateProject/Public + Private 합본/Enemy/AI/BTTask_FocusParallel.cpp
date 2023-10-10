
#include "Enemy/AI/BTTask_FocusParallel.h"

#include "Enemy/EnemyAIControllerBase.h"
#include "Enemy/EnemyCharacterBase.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Abilities/GameplayAbilityBase.h"
#include "Enemy/AI/BBKeys.h"

#include "BehaviorTree/BlackboardComponent.h"



UBTTask_FocusParallel::UBTTask_FocusParallel() : Super()
{
	static ConstructorHelpers::FClassFinder<UGameplayEffect> EffectClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Effects/Enemy/GE_Enemy_SlowWhileSearch");

	if (EffectClass.Succeeded())
	{
		EffectClassToApplyOnExecute = EffectClass.Class;
	}

	EffectTagToRemoveOnAbort = FGameplayTag::RequestGameplayTag(TEXT("Effect.Enemy.SlowWhileSearch"));

	NodeName = TEXT("TargetActor에 포커스");
}



EBTNodeResult::Type UBTTask_FocusParallel::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyCharacterBase* EnemyCharacter = OwnerComp.GetAIOwner()->GetPawn<AEnemyCharacterBase>())
	{
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeys::TargetActor));

		if (!IsValid(TargetActor)) return EBTNodeResult::Failed;

		OwnerComp.GetAIOwner()->SetFocus(TargetActor);

		if (IsValid(EnemyCharacter->GetAbilitySystemComponent()) && IsValid(EffectClassToApplyOnExecute))
		{
			const FGameplayEffectSpecHandle SpecHandle = EnemyCharacter->GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClassToApplyOnExecute, 1.0f, EnemyCharacter->GetAbilitySystemComponent()->MakeEffectContext());
			
			EnemyCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			UE_LOG(LogTemp, Log, TEXT("Execute!!"));
		}
	}

	return EBTNodeResult::InProgress;
}



EBTNodeResult::Type UBTTask_FocusParallel::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyCharacterBase* EnemyCharacter = OwnerComp.GetAIOwner()->GetPawn<AEnemyCharacterBase>())
	{
		OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);

		if (IsValid(EnemyCharacter->GetAbilitySystemComponent()) && EffectTagToRemoveOnAbort.IsValid())
		{
			const FGameplayTagContainer TagContainer { EffectTagToRemoveOnAbort };

			EnemyCharacter->GetAbilitySystemComponent()->RemoveActiveEffectsWithTags(TagContainer);
		}
	}

	return EBTNodeResult::Aborted;
}
