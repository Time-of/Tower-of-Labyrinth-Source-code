
#pragma once

#include "CoreMinimal.h"
#include "Enemy/Abilities/GameplayAbilityBase.h"
#include "GA_Cpp_Boss_Plant_SpawnRoots.generated.h"



/**
 * @author 이성수
 * @brief 식물 보스의 뿌리 소환 패턴 어빌리티 클래스입니다. **반드시 상속하여 사용해 EndAbility를 호출해 줄 것 !!!**
 * @since 2023-03-28
 */
UCLASS()
class GRADUATEPROJECT_API UGA_Cpp_Boss_Plant_SpawnRoots : public UGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UGA_Cpp_Boss_Plant_SpawnRoots();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;



protected:
	UPROPERTY(EditAnywhere, Category = "Ability|Plant")
	TSubclassOf<class AEnemyCharacterBase> RootEnemyClass;
};
