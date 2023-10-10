
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdateDistanceFromTarget.generated.h"



/**
 * @author 이성수
 * @brief 기존에 사용하던 타겟과의 거리 체크를 C++로 옮긴 클래스입니다.
 * @since 2023-03-10
 */
UCLASS()
class GRADUATEPROJECT_API UBTS_UpdateDistanceFromTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_UpdateDistanceFromTarget();



protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


};
