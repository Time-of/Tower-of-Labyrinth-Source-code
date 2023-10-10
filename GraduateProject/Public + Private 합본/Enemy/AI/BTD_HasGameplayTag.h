
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_HasGameplayTag.generated.h"




struct FHasGameplayTagMemory
{
	bool bBeforeResult;
};



/**
 * @author 이성수
 * @brief 게임플레이 태그를 가지고 있는지 판단하는 데코레이터입니다.
 * @see CalculateRawConditionValue() 참고.
 * @since 2023-02-02
 */
UCLASS()
class GRADUATEPROJECT_API UBTD_HasGameplayTag : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_HasGameplayTag();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;



protected:
	virtual uint16 GetInstanceMemorySize() const override;



	// 비교할 태그
	UPROPERTY(EditAnywhere, Category = "Tag")
	FGameplayTag GameplayTag;

	// true인 경우 비교할 태그를 가지고 있다면 true 반환
	UPROPERTY(EditAnywhere, Category = "Tag")
	bool bHasTag;
};
