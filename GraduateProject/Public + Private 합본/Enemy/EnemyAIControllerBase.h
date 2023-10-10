
#pragma once

#include "CoreMinimal.h"

#include "AIController.h"

#include "AbilitySystemInterface.h"
#include "Perception/AIPerceptionTypes.h"

#include "EnemyAIControllerBase.generated.h"



/**
 * @author 이성수
 * @brief 적 캐릭터의 AI 컨트롤러 베이스 클래스입니다.
 * @since 2023-01-31
 */
UCLASS()
class GRADUATEPROJECT_API AEnemyAIControllerBase : public AAIController, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEnemyAIControllerBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UAttributeSetBase* GetAttributeSetBase() const;

	virtual void OnPossess(APawn* InPawn) override;

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, const FAIStimulus Stimulus);

	void RunAI();

	void StopAI();



protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEnemyPawnDied(class AEnemyCharacterBase* Enemy);

	UFUNCTION()
	void OnPlayerDied(class APlayerCharacterBase* Player);

	bool bIsPlayerDied;

	class UAISenseConfig_Sight* SightSense;

	class UAISenseConfig_Touch* TouchSense;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyAI")
	class UBehaviorTree* BTAsset;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyAI")
	class UBlackboardData* BBAsset;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyAI")
	UAnimMontage* MontageOnPlayerDied;
};
