
#include "Enemy/State/BattleCruiser/BossState_BattleCruiser_Groggy.h"

#include "Enemy/Enemy_Boss_BattleCruiser.h"
#include "Enemy/Interface/EnemyHandler.h"
#include "Widget/Interface/BossHealthbarWidgetHandler.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



BossState_BattleCruiser_Groggy::BossState_BattleCruiser_Groggy() : BCRef(nullptr)
{
}



BossState_BattleCruiser_Groggy::BossState_BattleCruiser_Groggy(AEnemy_Boss_BattleCruiser* NewBattleCruiser) : BCRef(NewBattleCruiser)
{
}



BossState_BattleCruiser_Groggy::~BossState_BattleCruiser_Groggy()
{
}



void BossState_BattleCruiser_Groggy::Enter(AEnemy_Boss_Master* Boss)
{
	BCRef->ApplyForceMoveSpeedZeroEffect();

	BCRef->StartGroggy();
}



void BossState_BattleCruiser_Groggy::Exit(AEnemy_Boss_Master* Boss)
{
	BCRef->RemoveForceMoveSpeedZeroEffect();

	UUserWidget* HealthbarWidget = BCRef->GetHealthbarWidgetObject();

	if (HealthbarWidget != nullptr && HealthbarWidget->Implements<UBossHealthbarWidgetHandler>())
	{
		IBossHealthbarWidgetHandler::Execute_EndGuardChargingAnimation(HealthbarWidget);

		IEnemyHandler::Execute_SetGuardToFull(BCRef);
	}
}
