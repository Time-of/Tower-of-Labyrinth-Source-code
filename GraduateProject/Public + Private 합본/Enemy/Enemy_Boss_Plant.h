
#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy_Boss_Master.h"
#include "Enemy_Boss_Plant.generated.h"



/**
 * @author 이성수
 * @brief 식물 보스 몬스터 클래스입니다.
 * @since 2023-05-25
 */
UCLASS()
class GRADUATEPROJECT_API AEnemy_Boss_Plant : public AEnemy_Boss_Master
{
	GENERATED_BODY()

public:
	AEnemy_Boss_Plant();

	UFUNCTION()
	void OnPlantRootDied(AEnemyCharacterBase* Enemy);

	TArray<FVector> GetRootSpawnLocalLocations() const;



protected:
	UPROPERTY(EditAnywhere, Category = "Ability|Plant")
	TSubclassOf<UGameplayEffect> DecreaseGuardEffectClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability|Plant", Meta = (MakeEditWidget = true))
	TArray<FVector> RootSpawnLocationArray;
};
