
#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "Enemy/Enemy_Boss_Master.h"
#include "Enemy/State/BossStateBase.h"

#include "BossStateMachine.generated.h"



/**
 * @author 이성수
 * @brief 보스 몬스터의 상태머신 클래스입니다.
 * @since 2023-04-19
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GRADUATEPROJECT_API UBossStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:
	UBossStateMachine();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool ChangeState(const TSharedPtr<BossStateBase>& NewState);



protected:
	TSharedPtr<BossStateBase> CurrentState;

	UPROPERTY()
	AEnemy_Boss_Master* Boss;
};
