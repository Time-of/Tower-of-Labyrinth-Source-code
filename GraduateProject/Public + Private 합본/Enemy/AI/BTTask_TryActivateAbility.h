
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TryActivateAbility.generated.h"



/**
 * @author 이성수
 * @brief 어빌리티 사용을 시도하는 BTTask 입니다.
 * @details 어빌리티 사용에 성공한 경우에만 Successed를 반환하고, 실패한 경우에는 Failed를 반환합니다.
 * @details 그래서 대부분은 Failed인 상태입니다.
 * @see ExecuteTask() 참고.
 * @since 2023-02-01
 */
UCLASS()
class GRADUATEPROJECT_API UBTTask_TryActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TryActivateAbility();

	/**
	* @return 어빌리티 사용에 성공한 경우에만 Successed를 반환하고, 실패한 경우에는 Failed를 반환합니다.
	*/
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;



protected:
	UPROPERTY(EditAnywhere, Category = "Ability")
	bool bActivateByTags;

	UPROPERTY(EditAnywhere, Category = "Ability", meta = (EditCondition = "!bActivateByTags"))
	TSubclassOf<class UGameplayAbility> AbilityClass;

	UPROPERTY(EditAnywhere, Category = "Ability", meta = (EditCondition = "bActivateByTags"))
	FGameplayTagContainer AbilityTag;
};
