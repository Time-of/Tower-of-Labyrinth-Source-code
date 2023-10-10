
#pragma once

#include "CoreMinimal.h"

#include "Enemy/State/BossStateBase.h"



/**
 * @author 이성수
 * @brief 전투순양함 보스의 공격 상태입니다.
 * @since 2023-04-19
 */
class GRADUATEPROJECT_API BossState_BattleCruiser_Attack : public BossStateBase
{
public:
	BossState_BattleCruiser_Attack();
	BossState_BattleCruiser_Attack(class AEnemy_Boss_BattleCruiser* NewBattleCruiser);
	~BossState_BattleCruiser_Attack();

	void Enter(AEnemy_Boss_Master* Boss) override;

	void Exit(AEnemy_Boss_Master* Boss) override;

	static TSharedPtr<BossStateBase> Create(class AEnemy_Boss_BattleCruiser* NewBattleCruiser)
	{
		return MakeShareable(new BossState_BattleCruiser_Attack(NewBattleCruiser));
	}



private:
	UPROPERTY()
	class AEnemy_Boss_BattleCruiser* BCRef;
};
