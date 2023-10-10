
#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy_Boss_Master.h"
#include "Enemy_Boss_Fighter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPhase3Entered);



/**
 * @author 이성수
 * @brief 파이터 보스입니다.
 * @since 2023-05-16
 */
UCLASS()
class GRADUATEPROJECT_API AEnemy_Boss_Fighter : public AEnemy_Boss_Master
{
	GENERATED_BODY()

public:
	AEnemy_Boss_Fighter();



	UPROPERTY(BlueprintAssignable, Category = "Enemy|Pattern|Phase")
	FOnPhase3Entered OnPhase3EnteredDelegate;



protected:
	void CheckPhase2() override;

	UFUNCTION(BlueprintCallable)
	void SetUsingSword(bool NewActive);

	UFUNCTION(BlueprintImplementableEvent)
	void PostUsingSwordChanged(bool IsUsingSword);



	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy|Pattern|Phase")
	bool bPhase3Entered;

	/** @brief 3페이즈 진입 체력 비율 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enemy|Pattern|Phase")
	float Phase3EnterHealthRate;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Pattern|Phase")
	bool bUsingSword;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enemy|Pattern|Phase")
	TSubclassOf<UGameplayEffect> AblazeEffectClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enemy|Pattern|Phase")
	TSubclassOf<UGameplayEffect> ElectricEffectClass;
};
