
#include "Enemy/State/BattleCruiser/BossState_BattleCruiser_Attack.h"

#include "Enemy/Enemy_Boss_BattleCruiser.h"

#include "GameFramework/CharacterMovementComponent.h"



BossState_BattleCruiser_Attack::BossState_BattleCruiser_Attack() : BCRef(nullptr)
{
}



BossState_BattleCruiser_Attack::BossState_BattleCruiser_Attack(AEnemy_Boss_BattleCruiser* NewBattleCruiser) : BCRef(NewBattleCruiser)
{
}



BossState_BattleCruiser_Attack::~BossState_BattleCruiser_Attack()
{
}



void BossState_BattleCruiser_Attack::Enter(AEnemy_Boss_Master* Boss)
{
	BCRef->ApplyForceMoveSpeedZeroEffect();
}



void BossState_BattleCruiser_Attack::Exit(AEnemy_Boss_Master* Boss)
{
	BCRef->RemoveForceMoveSpeedZeroEffect();
	
	BCRef->HandleCancelCancelableAbilities();
}
