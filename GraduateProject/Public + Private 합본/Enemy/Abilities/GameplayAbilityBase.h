
#pragma once

#include "CoreMinimal.h"

#include "Abilities/GameplayAbility.h"

#include "GraduateProject/GraduateProject.h"

#include "GameplayAbilityBase.generated.h"



/**
 * @author 이성수
 * @brief 캐릭터가 사용하는 게임플레이 어빌리티의 베이스 클래스 (Enemy라 써 있지만, 모든 캐릭터 공용)
 * @details 사용하기 편리하게 몇몇 태그를 기본값으로 가지고 시작하는 클래스.
 * @details 굳이 사용하지 않아도 무방하다.
 * @since 2023-01-31
 */
UCLASS()
class GRADUATEPROJECT_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbilityBase();

	void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) final;

	void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) final;



protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnInputButtonPressed(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActivationInfo& ActivationInfo);

	UFUNCTION(BlueprintImplementableEvent)
	void OnInputButtonReleased(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActivationInfo& ActivationInfo);
};
