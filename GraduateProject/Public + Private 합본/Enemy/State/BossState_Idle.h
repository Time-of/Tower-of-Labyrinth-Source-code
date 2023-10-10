
#pragma once

#include "CoreMinimal.h"

#include "Enemy/State/BossStateBase.h"



/**
 * @author 이성수
 * @brief 보스 상태 중 Idle 상태 클래스입니다.
 * @since 2023-04-19
 */
class GRADUATEPROJECT_API BossState_Idle : public BossStateBase
{
public:
	BossState_Idle();

	~BossState_Idle();

	void Enter(AEnemy_Boss_Master* Boss) override;
	
	void Exit(AEnemy_Boss_Master* Boss) override;

	static TSharedPtr<BossStateBase> Create()
	{
		return MakeShareable(new BossState_Idle());
	}
};
