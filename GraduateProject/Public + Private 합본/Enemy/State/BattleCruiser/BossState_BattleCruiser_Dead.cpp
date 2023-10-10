
#include "Enemy/State/BattleCruiser/BossState_BattleCruiser_Dead.h"

#include "Enemy/Enemy_Boss_BattleCruiser.h"



BossState_BattleCruiser_Dead::BossState_BattleCruiser_Dead() : BCRef(nullptr)
{
}



BossState_BattleCruiser_Dead::BossState_BattleCruiser_Dead(AEnemy_Boss_BattleCruiser* NewBattleCruiser) : BCRef(NewBattleCruiser)
{
}



BossState_BattleCruiser_Dead::~BossState_BattleCruiser_Dead()
{
}



void BossState_BattleCruiser_Dead::Enter(AEnemy_Boss_Master* Boss)
{
	BCRef->ApplyForceMoveSpeedZeroEffect();
}



void BossState_BattleCruiser_Dead::Exit(AEnemy_Boss_Master* Boss)
{

}
