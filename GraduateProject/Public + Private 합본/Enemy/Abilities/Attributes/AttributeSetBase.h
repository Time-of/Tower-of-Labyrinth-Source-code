
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"

#include "AbilitySystemComponent.h"

#include "AttributeSetBase.generated.h"



#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)



/**
 * @author 이성수
 * @brief 캐릭터가 가지는 능력치 종류들을 담당하는 클래스 (Enemy라 써 있지만, 작명 실수. 모든 캐릭터 공용)
 * @details 캐릭터의 어트리뷰트 셋 베이스 클래스
 * @since 2023-01-31
 */
UCLASS()
class GRADUATEPROJECT_API UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	/**
	* @brief 어트리뷰트가 변경될 때 호출되어, 최대 체력이 변경될 때 현재 체력을 함께 조정하기 위해 오버라이드한 기능
	* @details 최대 체력이 줄어들었다면, 현재 체력이 변경된 최대 체력보다 많은 경우 변경된 최대 체력으로 조정
	* @details 최대 체력이 늘어났다면, 현재 체력을 늘어난 체력에 비례하여 조정
	*/
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/**
	* @brief 주로 Damage 처리 및 CharacterBase의 On[Attribute]Changed를 컨트롤하는 역할
	* @see UGEC_DamageExecution 참고.
	*/
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;



	/**
	* @brief 레벨
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Level)

	/**
	* @brief 체력
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Health)

	/**
	* @brief 최대 체력
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MaxHealth)

	/**
	* @brief **피해량 계산을 위해 사용되는 __임시 변수__**
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Damage)

	/**
	* @brief **피해량 계산을 위해 사용되는 __임시 변수__**
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData DamageToDirectHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, DamageToDirectHealth)

	/**
	* @brief 힘 수치, 피해량 계산에 사용됨
	* @see UGEC_DamageExecution 참고.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Strength)

	/**
	* @brief 가드 게이지,  > 0인 경우 피해량 계산 시 피해량에 DamageMultOnGuardExists 만큼 곱해진다.
	* @see UGEC_DamageExecution 참고.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Guard;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Guard)

	/**
	* @brief 내부적으로만 쓰이는 최대 가드입니다. 따로 설정할 필요 없이 기본 Guard값에 따라 자동으로 설정됨
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData InternalMaxGuard;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, InternalMaxGuard)

	/**
	* @brief 가드 게이지가 존재할 때 받는 피해량 배율
	* @see UGEC_DamageExecution 참고.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData DamageMultOnGuardExists;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, DamageMultOnGuardExists)

	/**
	* @brief 가드 게이지가 받는 피해 배율
	* @details 만약 0.8이라면, 가드 게이지는 원본 피해의 80% 만큼 깎인다.
	* @see UGEC_DamageExecution 참고.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData GuardDamageTakenMult;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, GuardDamageTakenMult)

	/**
	* @brief 이동 속도
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MoveSpeed)

	/**
	* @brief 공격 속도, 몽타주 플레이 시 GetAttackSpeed()를 사용해 Play Rate에 넣을 것.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, AttackSpeed)

	/**
	* @brief 방어력, 이론 상 최대 방어력: 1072 (30%)
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Defense)
};
