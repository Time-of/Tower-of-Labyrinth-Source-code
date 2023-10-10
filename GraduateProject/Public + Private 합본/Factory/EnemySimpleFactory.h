
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include <functional>

#include "EnemySimpleFactory.generated.h"



/**
 * @author 이성수
 * @brief 적 캐릭터를 만들어주는 심플 팩토리 클래스입니다.
 * @since 2023-03-28
 */
UCLASS()
class GRADUATEPROJECT_API UEnemySimpleFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** @brief 적 캐릭터를 스폰합니다. */
	static class AEnemyCharacterBase* SpawnEnemy(const AActor* WorldContext,
		TSubclassOf<class AEnemyCharacterBase> EnemyClassToSpawn,
		const FVector& Location, const FRotator& Rotation, APawn* Instigator, AActor* Owner,
		const FScriptDelegate& EnemyDiedCallback);
};
