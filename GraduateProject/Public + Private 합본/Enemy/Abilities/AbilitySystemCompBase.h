
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemCompBase.generated.h"



/**
 * @author 이성수
 * @brief 캐릭터가 사용하는 어빌리티 시스템 컴포넌트 (Enemy라 써 있지만, 모든 캐릭터 공용)
 * @details 어빌리티 및 스타트업 이펙트(즉, 패시브 효과)의 중복 지급을 막기 위한 변수 선언됨.
 * @since 2023-01-31
 */
UCLASS()
class GRADUATEPROJECT_API UAbilitySystemCompBase : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UAbilitySystemCompBase();

	/**
	* @brief 어빌리티가 주어졌는가?
	*/
	bool bAbilitiesWereGiven;

	/**
	* @brief 스타트업 이펙트(즉, 패시브 효과)가 적용되었는가?
	*/
	bool bIsStartupEffectsApplied;
};
