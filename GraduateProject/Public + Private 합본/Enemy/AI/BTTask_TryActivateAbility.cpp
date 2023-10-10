
#include "Enemy/AI/BTTask_TryActivateAbility.h"

#include "Enemy/EnemyAIControllerBase.h"
#include "Enemy/EnemyCharacterBase.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Abilities/GameplayAbilityBase.h"



UBTTask_TryActivateAbility::UBTTask_TryActivateAbility() : Super(), bActivateByTags(false)
{
	NodeName = TEXT("어빌리티 활성 시도");
}



EBTNodeResult::Type UBTTask_TryActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyCharacterBase* EnemyCharacter = OwnerComp.GetAIOwner()->GetPawn<AEnemyCharacterBase>())
	{
		UAbilitySystemComponent* ASC = EnemyCharacter->GetAbilitySystemComponent();

		if (IsValid(ASC))
		{
			bool bSuccessed = bActivateByTags ? ASC->TryActivateAbilitiesByTag(AbilityTag) : ASC->TryActivateAbilityByClass(AbilityClass);

			return bSuccessed ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Failed;
}



FString UBTTask_TryActivateAbility::GetStaticDescription() const
{
	return FString::Printf(TEXT("TryActivate: %s"), bActivateByTags ? *AbilityTag.ToString() : (IsValid(AbilityClass) ? *AbilityClass->GetName() : TEXT("없음")));
}
