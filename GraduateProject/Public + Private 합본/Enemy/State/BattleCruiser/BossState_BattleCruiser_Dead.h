
#pragma once

#include "CoreMinimal.h"

#include "Enemy/State/BossStateBase.h"



/**
 * @author 이성수
 * @brief 전투순양함 보스의 사망 상태입니다.
 * @since 2023-04-19
 */
class GRADUATEPROJECT_API BossState_BattleCruiser_Dead : public BossStateBase
{
public:
public:
	BossState_BattleCruiser_Dead();
	BossState_BattleCruiser_Dead(class AEnemy_Boss_BattleCruiser* NewBattleCruiser);
	~BossState_BattleCruiser_Dead();

	void Enter(AEnemy_Boss_Master* Boss) override;

	void Exit(AEnemy_Boss_Master* Boss) override;

	static TSharedPtr<BossStateBase> Create(class AEnemy_Boss_BattleCruiser* NewBattleCruiser)
	{
		return MakeShareable(new BossState_BattleCruiser_Dead(NewBattleCruiser));
	}



private:
	UPROPERTY()
	class AEnemy_Boss_BattleCruiser* BCRef;
};
