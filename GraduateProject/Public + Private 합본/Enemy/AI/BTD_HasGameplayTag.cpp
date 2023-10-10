
#include "Enemy/AI/BTD_HasGameplayTag.h"

#include "Enemy/EnemyAIControllerBase.h"
#include "Enemy/EnemyCharacterBase.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Abilities/GameplayAbilityBase.h"



UBTD_HasGameplayTag::UBTD_HasGameplayTag() : Super(), bHasTag(true)
{
	NodeName = TEXT("태그를 가지고 있는가?");

	bAllowAbortLowerPri = true;
	bAllowAbortChildNodes = true;
}



bool UBTD_HasGameplayTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (AEnemyCharacterBase* EnemyCharacter = OwnerComp.GetAIOwner()->GetPawn<AEnemyCharacterBase>())
	{
		if (UAbilitySystemComponent* ASC = EnemyCharacter->GetAbilitySystemComponent())
		{
			return bHasTag ? (ASC->GetTagCount(GameplayTag) > 0) : (ASC->GetTagCount(GameplayTag) <= 0);
		}
	}

	return false;
}



FString UBTD_HasGameplayTag::GetStaticDescription() const
{
	return FString::Printf(TEXT("태그 %s: %s"),
		bHasTag ? *FString(TEXT("가지고 있다면")) : *FString(TEXT("가지지 않았다면")),
		GameplayTag.IsValid() ? *GameplayTag.GetTagName().ToString() : *FString(TEXT("없음")));
}



void UBTD_HasGameplayTag::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FHasGameplayTagMemory* BeforeResultMemory = CastInstanceNodeMemory<FHasGameplayTagMemory>(NodeMemory);
	bool bResult = CalculateRawConditionValue(OwnerComp, NodeMemory);

	// 이전 결과값과 다르다면 Abort 실행
	if (bResult != BeforeResultMemory->bBeforeResult)
	{
		BeforeResultMemory->bBeforeResult = bResult;
		OwnerComp.RequestExecution(this);
		UE_LOG(LogTemp, Warning, TEXT("이전 결과값과 달라 Abort 실행됨!!"));
	}
}



void UBTD_HasGameplayTag::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	FHasGameplayTagMemory* BeforeResultMemory = CastInstanceNodeMemory<FHasGameplayTagMemory>(NodeMemory);
	BeforeResultMemory->bBeforeResult = CalculateRawConditionValue(OwnerComp, NodeMemory);
}



uint16 UBTD_HasGameplayTag::GetInstanceMemorySize() const
{
	return sizeof(FHasGameplayTagMemory);
}
