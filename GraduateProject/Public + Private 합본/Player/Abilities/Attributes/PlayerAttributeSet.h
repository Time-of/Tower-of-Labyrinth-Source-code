
#pragma once

#include "CoreMinimal.h"
#include "Enemy/Abilities/Attributes/AttributeSetBase.h"
#include "PlayerAttributeSet.generated.h"



#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)



/**
 * @author 이성수
 * @brief 플레이어의 어트리뷰트 셋 클래스입니다.
 * @since 2023-03-04
 */
UCLASS()
class GRADUATEPROJECT_API UPlayerAttributeSet : public UAttributeSetBase
{
	GENERATED_BODY()

public:
	/** @brief 기본적으로 UEnemyAttributeSetBase가 하는 일을 똑같이 수행하고, 추가로 SkillMana와 MaxSkillMana에 관한 처리를 수행합니다. */
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;



	/** @brief 스킬 게이지 */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Player")
	FGameplayAttributeData SkillMana;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, SkillMana)

	/** @brief 최대 스킬 게이지 */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Player")
	FGameplayAttributeData MaxSkillMana;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MaxSkillMana)

	/** @brief 스킬 게이지 사용 비율 (1.0일 때 100%) */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Player")
	FGameplayAttributeData SkillManaUsageRate;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, SkillManaUsageRate)

	/** @brief 스킬 마나를 사용하기 위한 **임시 변수!!** */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Player")
	FGameplayAttributeData SkillManaAmountToUse;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, SkillManaAmountToUse)

	/** @brief 공격 거리 배율 */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Player")
	FGameplayAttributeData AttackRangeRate;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, AttackRangeRate)
};
