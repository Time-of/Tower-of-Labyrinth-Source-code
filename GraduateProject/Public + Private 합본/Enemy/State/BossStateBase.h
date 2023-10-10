
#pragma once

#include "CoreMinimal.h"

#include "Enemy/Enemy_Boss_Master.h"



/**
 * @author 이성수
 * @brief 보스 몬스터의 상태 베이스 클래스입니다.
 * @since 2023-04-19
 */
class GRADUATEPROJECT_API BossStateBase
{
public:
	BossStateBase();

	virtual ~BossStateBase();

	virtual void Tick(AEnemy_Boss_Master* Boss, float DeltaSeconds);

	virtual void Enter(AEnemy_Boss_Master* Boss);

	virtual void Exit(AEnemy_Boss_Master* Boss);

	bool bNeedUpdateTick;
};
