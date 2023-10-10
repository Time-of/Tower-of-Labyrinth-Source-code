
#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy_Boss_Master.h"
#include "Enemy_Boss_BattleCruiser.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLandCompleteDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGroggyStartDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGroggyCompleteDelegate);



UENUM(BlueprintType)
enum class EBattleCruiserState : uint8
{
	NONE, IDLE, GROGGY, ATTACK, DEAD
};



/**
 * @author 이성수
 * @brief QA 작업 중 리메이크한 전투순양함 보스 몬스터입니다.
 * @details 참고: 이 클래스에서는 RefillGuard Effect를 사용하지 않음
 * @since 2023-04-19
 */
UCLASS()
class GRADUATEPROJECT_API AEnemy_Boss_BattleCruiser : public AEnemy_Boss_Master
{
	GENERATED_BODY()
	
public:
	AEnemy_Boss_BattleCruiser();

	void BeginPlay() override;

	void ApplyForceMoveSpeedZeroEffect();

	void RemoveForceMoveSpeedZeroEffect();

	UFUNCTION(BlueprintCallable)
	void StartLand(bool bReverse);

	void StartGroggy();

	void InternalSetCurrentStateEnum(EBattleCruiserState NewStateEnum)
	{
		CurrentStateEnum = NewStateEnum;
	}

	void DieAction() final;



	UPROPERTY(BlueprintAssignable)
	FOnLandCompleteDelegate OnLandComplete;



protected:
	UFUNCTION()
	void OnLandTimelineUpdate(float Value);

	UFUNCTION()
	void OnLandEnded();

	UFUNCTION()
	void OnGroggyStarted();

	UFUNCTION()
	void OnGroggyEnded();

	void OnGuardChanged_Implementation(float DeltaGuard, const FGameplayTagContainer& SourceTags) final;

	/** @brief 강제로 그로기 상태를 해제합니다. 2페이즈 진입 시 사용됩니다. */
	UFUNCTION()
	void ForceEndGroggy();

	UFUNCTION()
	void ChangeStateToIdleOnGroggyForceEnded();



	UPROPERTY(BlueprintAssignable)
	FOnGroggyStartDelegate OnGroggyStart;

	UPROPERTY(BlueprintAssignable)
	FOnGroggyCompleteDelegate OnGroggyComplete;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EBattleCruiserState CurrentStateEnum;

	/** @brief 그로기 상태의 지속시간입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Pattern")
	float GroggyDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* HitBoxComp;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> ForceMoveSpeedZeroEffectClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBossStateMachine_BattleCruiser* StateMachineComp;

	UPROPERTY()
	class UTimelineComponent* LandTimelineComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Pattern")
	UCurveFloat* LandCurveFloat;

	class FOnTimelineFloat LandTimelineUpdateEvent;

	class FOnTimelineEvent LandTimelineFinishedEvent;

	FTimerHandle GroggyTimerHandle;
};
