
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FocusParallel.generated.h"



/**
 * @author 이성수
 * @brief Execute 시에 TargetActor에 SetFocus하고, Abort 시에 ClearFocus하는 BTTask입니다.
 * @details EffectClassToApplyOnExecute가 있다면 Execute 시에 적용합니다.
 * @details EffectTagToRemoveOnAbort가 있다면 Abort 시에 해당 태그가 있는 이펙트를 모두 제거합니다.
 * @since 2023-02-14
 */
UCLASS()
class GRADUATEPROJECT_API UBTTask_FocusParallel : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FocusParallel();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;



protected:
	/**
	* @brief ExecuteTask시에 적용할 이펙트 클래스
	*/
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameplayEffect> EffectClassToApplyOnExecute;

	/**
	* @brief AbortTask 시에 제거할 태그
	*/
	UPROPERTY(EditAnywhere)
	FGameplayTag EffectTagToRemoveOnAbort;
};
