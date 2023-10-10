
#include "Enemy/State/BossStateMachine_BattleCruiser.h"

#include "Enemy/State/BossStateBase.h"
#include "Enemy/State/BossState_Idle.h"
#include "Enemy/State/BattleCruiser/BossState_BattleCruiser_Attack.h"
#include "Enemy/State/BattleCruiser/BossState_BattleCruiser_Dead.h"
#include "Enemy/State/BattleCruiser/BossState_BattleCruiser_Groggy.h"



UBossStateMachine_BattleCruiser::UBossStateMachine_BattleCruiser() : Super(),
	BattleCruiserRef(nullptr)
{

}



void UBossStateMachine_BattleCruiser::BeginPlay()
{
	Super::BeginPlay();

	
}



void UBossStateMachine_BattleCruiser::ChangeStateByEnum(EBattleCruiserState CurrentStateEnum, EBattleCruiserState NewStateEnum)
{
	if (CurrentStateEnum == NewStateEnum || !IsValid(BattleCruiserRef)) return;

	bool bResult = false;
	FString DebugString = TEXT("NONE");

	switch (NewStateEnum)
	{
		case EBattleCruiserState::IDLE:
			bResult = ChangeState(BossState_Idle::Create());
			DebugString = TEXT("IDLE");
			break;
		case EBattleCruiserState::GROGGY:
			bResult = ChangeState(BossState_BattleCruiser_Groggy::Create(BattleCruiserRef));
			DebugString = TEXT("GROGGY");
			break;
		case EBattleCruiserState::ATTACK:
			bResult = ChangeState(BossState_BattleCruiser_Attack::Create(BattleCruiserRef));
			DebugString = TEXT("ATTACK");
			break;
		case EBattleCruiserState::DEAD:
			bResult = ChangeState(BossState_BattleCruiser_Dead::Create(BattleCruiserRef));
			DebugString = TEXT("DEAD");
			break;
		case EBattleCruiserState::NONE:
		default:
			return;
	}

	if (bResult == true)
	{
		BattleCruiserRef->InternalSetCurrentStateEnum(NewStateEnum);

		UE_LOG(LogTemp, Warning, TEXT("전투순양함 현재 상태: %s"), *DebugString);
	}
}



void UBossStateMachine_BattleCruiser::SetupOwnerBoss(AEnemy_Boss_BattleCruiser* NewOwnerBoss)
{
	BattleCruiserRef = NewOwnerBoss;
}
