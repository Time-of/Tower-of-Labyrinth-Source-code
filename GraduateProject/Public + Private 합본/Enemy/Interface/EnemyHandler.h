
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyHandler.generated.h"



UINTERFACE(MinimalAPI)
class UEnemyHandler : public UInterface
{
	GENERATED_BODY()
};



/**
 * @author 이성수
 * @brief 적 액터들에게 공통적으로 사용되는 기능을 모아 놓은 인터페이스입니다.
 * @since 2023-01-27
 */
class GRADUATEPROJECT_API IEnemyHandler
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EnemyHandler|State|Attributes")
	void SetGuardToFull();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EnemyHandler|Attack")
	TSubclassOf<UGameplayEffect> GetAdditionalAttackEffectOnTarget() const;
};
