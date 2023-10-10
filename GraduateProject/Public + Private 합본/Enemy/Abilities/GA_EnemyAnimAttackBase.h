
#pragma once

#include "CoreMinimal.h"
#include "Enemy/Abilities/GameplayAbilityBase.h"
#include "GA_EnemyAnimAttackBase.generated.h"


/**
 * @author 이성수
 * @brief 적 캐릭터의 '애니메이션 기반 공격' 어빌리티 베이스 클래스
 * @details 애니메이션 노티파이 스테이트인 'NS_EnemyAttack'과 함께 동작합니다
 * @see UNS_EnemyAttack.h 참고.
 * @since 2023-09-24
 */
UCLASS()
class GRADUATEPROJECT_API UGA_EnemyAnimAttackBase : public UGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	UGA_EnemyAnimAttackBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnCancelled();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCompleted();

	UFUNCTION()
	void EventReceived(FGameplayEventData EventData);

	UFUNCTION(BlueprintImplementableEvent)
	void K_EventReceived(FGameplayEventData EventData);

	void ApplyEffectClassToTarget(TSubclassOf<UGameplayEffect> EffectClass, FGameplayEventData& EventData, FGameplayTag& DataTag);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GA|Enemy|AnimAttackBase")
	UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GA|Enemy|AnimAttackBase")
	float LockAbilityEffectDuration;

	UPROPERTY()
	FGameplayTag EnemyAbilityHitTag;

	UPROPERTY()
	FGameplayTag DamageDataTag;

	UPROPERTY()
	FGameplayTag LockAbilityDataTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GA|Enemy|AnimAttackBase")
	TArray<TSubclassOf<UGameplayEffect>> EffectClassesAppliedOnTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GA|Enemy|AnimAttackBase")
	TSubclassOf<UGameplayEffect> LockAbilityEffectClass;
};
