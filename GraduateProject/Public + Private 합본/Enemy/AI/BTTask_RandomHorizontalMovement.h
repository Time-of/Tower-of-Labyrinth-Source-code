
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RandomHorizontalMovement.generated.h"



/**
 * @author 이성수
 * @brief 좌우 중 무작위 방향으로 **Abort되기 전까지** 이동합니다.
 * @since 2023-02-14
 */
UCLASS()
class GRADUATEPROJECT_API UBTTask_RandomHorizontalMovement : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RandomHorizontalMovement();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;



protected:
	float Direction;

	float BackwardMovementInput;

	/** @brief 무작위로 뒤쪽 방향으로도 이동할 것인지 여부 */
	UPROPERTY(EditAnywhere)
	bool bAlsoRandomMoveBackward;
};
