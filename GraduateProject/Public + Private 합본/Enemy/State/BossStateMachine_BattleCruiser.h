
#pragma once

#include "CoreMinimal.h"

#include "Enemy/State/BossStateMachine.h"
#include "Enemy/Enemy_Boss_BattleCruiser.h"

#include "BossStateMachine_BattleCruiser.generated.h"



/**
 * @author 이성수
 * @brief 전투순양함 보스의 상태 머신 클래스입니다.
 * @since 2023-04-19
 */
UCLASS()
class GRADUATEPROJECT_API UBossStateMachine_BattleCruiser : public UBossStateMachine
{
	GENERATED_BODY()

public:
	UBossStateMachine_BattleCruiser();

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ChangeStateByEnum(EBattleCruiserState CurrentStateEnum, EBattleCruiserState NewStateEnum);

	void SetupOwnerBoss(class AEnemy_Boss_BattleCruiser* NewOwnerBoss);



private:
	UPROPERTY()
	class AEnemy_Boss_BattleCruiser* BattleCruiserRef;
};
