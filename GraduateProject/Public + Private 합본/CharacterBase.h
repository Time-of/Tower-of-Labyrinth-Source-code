
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"

#include "CharacterBase.generated.h"

struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedDelegate, float, CurrentHealth);



/**
 * @author 이성수
 * @brief 어트리뷰트를 사용하는 **공통된 캐릭터 베이스** 클래스입니다.
 * @details 플레이어 캐릭터와 적 캐릭터가 공통으로 사용합니다.
 * @since 2023-02-06
 */
UCLASS()
class GRADUATEPROJECT_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual void PostInitProperties() override;

protected:
	virtual void BeginPlay() override;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UAttributeSetBase* GetAttributeSetBase() const;

	virtual void PossessedBy(AController* NewController) override;

	/**
	* @brief 캐릭터가 살아있는지를 알 수 있는 함수
	* @return Health 어트리뷰트가 > 0인 경우 true, 아니라면 false
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|State")
	virtual bool IsAlive() const;

	/**
	* @brief 캐릭터 레벨을 가져옵니다.
	* @return Level 어트리뷰트 값
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|State|Attributes")
	int32 GetCharacterLevel() const;

	/**
	* @brief 현재 체력을 가져옵니다.
	* @return Health 어트리뷰트 값
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|State|Attributes")
	float GetHealth() const;

	/**
	* @brief 최대 체력을 가져옵니다.
	* @return MaxHealth 어트리뷰트 값
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|State|Attributes")
	float GetMaxHealth() const;

	/**
	* @brief Strength(힘)을 가져옵니다.
	* @return Strength 어트리뷰트 값
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|State|Attributes")
	float GetStrength() const;

	/**
	* @brief Guard(가드)를 가져옵니다.
	* @return Guard 어트리뷰트 값
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|State|Attributes")
	float GetGuard() const;

	/**
	* @brief 이동 속도를 가져옵니다.
	* @return MoveSpeed 어트리뷰트 값
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|State|Attributes")
	float GetMoveSpeed() const;

	/**
	* @brief 공격 속도를 가져옵니다.
	* @return AttackSpeed 어트리뷰트 값
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|State|Attributes")
	float GetAttackSpeed() const;

	/**
	* @brief 방어력을 가져옵니다.
	* @return Defense 어트리뷰트 값
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|State|Attributes")
	float GetDefense() const;

	/**
	* @brief OnDamaged()를 호출하기 위한 기능
	* @see OnDamaged() 참고.
	*/
	void HandleDamage(float DamageAmount, const FHitResult& HitResult, const FGameplayTagContainer& DamageTags, ACharacterBase* InstigatorCharacter, AActor* DamageCauser);
	
	/**
	* @brief OnHealthChanged()를 호출시키며 OnHealthChangedDelegate를 브로드캐스트합니다.
	* @see OnHealthChanged() 및 OnHealthChangedDelegate 참고.
	*/
	void HandleHealthChanged(float DeltaHealth, const FGameplayTagContainer& SourceTags);

	/**
	 * @brief 체력이 변경되었을 때 호출
	 * 
	 * @param DeltaHealth 체력의 변화율 (체력이 3 깎였다면, -3)
	 * @param SourceTags 원인 태그
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "CharacterBase|Attribute")
	void OnHealthChanged(float DeltaHealth, const FGameplayTagContainer& SourceTags);

	/**
	 * @brief 가드가 변경되었을 때 호출
	 *
	 * @param DeltaGuard 가드의 변화율
	 * @param SourceTags 원인 태그
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "CharacterBase|Attribute")
	void OnGuardChanged(float DeltaGuard, const FGameplayTagContainer& SourceTags);

	/**
	 * @brief 힘이 변경되었을 때 호출
	 *
	 * @param DeltaStrength 힘의 변화율
	 * @param SourceTags 원인 태그
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "CharacterBase|Attribute")
	void OnStrengthChanged(float DeltaStrength, const FGameplayTagContainer& SourceTags);

	/**
	 * @brief 방어력이 변경되었을 때 호출
	 *
	 * @param DeltaDefense 방어력의 변화율
	 * @param SourceTags 원인 태그
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "CharacterBase|Attribute")
	void OnDefenseChanged(float DeltaDefense, const FGameplayTagContainer& SourceTags);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase|Attribute")
	void SetHealth(float NewHealth);



	/** @brief 무기 장착을 해제합니다. */
	virtual void UnEquipWeapon();

	/** @brief 방어구 장착을 해제합니다. */
	void UnEquipArmor();

	/** @brief 무기를 장착합니다. 일반적으로 ItemBase 계열의 클래스에서 TryEquipItem을 사용합니다. */
	virtual void EquipWeapon(class UItem_WeaponBase* WeaponToEquip);

	/** @brief 방어구를 장착합니다. 일반적으로 ItemBase 계열의 클래스에서 TryEquipItem을 사용합니다. */
	void EquipArmor(class UItem_ArmorBase* ArmorToEquip);

	/** @brief 스킬 아이템으로 스킬 사용을 시도합니다. */
	//bool TryUseSkillBySkillItem(const FPrimaryAssetId& SkillToUse);

	class UItem_WeaponBase* GetEquippedWeapon() const;

	class UItem_ArmorBase* GetEquippedArmor() const;

	/**
	 * @brief 공격 시 추가로 적용할 이펙트를 설정합니다.
	 * @see AdditionalAttackEffectOnTarget 참고.
	 *
	 * @param NewEffectClass 새롭게 설정할 이펙트 클래스입니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|Attack")
	void SetAdditionalAttackEffectOnTarget(TSubclassOf<UGameplayEffect> NewEffectClass) { AdditionalAttackEffectOnTarget = NewEffectClass; }

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedDelegate OnHealthChangedDelegate;



protected:
	/**
	 * @brief 스턴 태그가 붙었거나 떼어질 때 호출됩니다.
	 * @details 여기에서 실제로 스턴 작업을 진행
	 * @details 스턴 태그가 있다면, 어빌리티를 취소시킵니다.
	 */
	virtual void OnStunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	/**
	 * @brief 캐릭터의 사망 기능을 다룹니다.
	 * @details 기본적으로 하는 역할은, 캡슐 콜리전 끄기, 사망 몽타주 재생, 어빌리티 제거, 사망 시 사라지는 이펙트 제거입니다.
	 */
	void HandleDie();

	/** 
	* @brief 캐릭터가 사망 시 수정되거나 행동할 것들은 이 함수에서 구현합니다.
	* @see HandleDie() 참고.
	*/
	virtual void DieAction();

	/**
	 * @brief 기본 어트리뷰트(즉, 스탯)를 초기화합니다.
	 * @see DefaultAttribute 참고.
	 */
	virtual void InitializeDefaultAttribute();

	/**
	 * @brief 어빌리티를 추가합니다.
	 * @see Abilities, UAbilitySystemCompBase 참고.
	 */
	void AddAbilities();

	/**
	 * @brief 어빌리티를 제거합니다.
	 * @details HandleDie()에서 호출됩니다.
	 */
	void RemoveAbilities();

	/**
	 * @brief 시작 효과(즉, 패시브 효과)를 추가합니다.
	 * @see StartupEffects 참고.
	 */
	void AddStartupEffects();

	/**
	 * @brief Guard 어트리뷰트를 새 값으로 설정합니다.
	 * @param NewGuard 새롭게 설정할 값
	 */
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|Attribute")
	virtual void SetGuardGuage(float NewGuard);

	/**
	 * @brief 캐릭터가 피해를 받았을 때 호출됩니다.
	 * @details 단, 기획 상 최대 체력 비례 피해에는 호출되지 않습니다.
	 * @see UAttributeSetBase 참고.
	 * 
	 * @param DamageAmount 받은 피해량
	 * @param HitResult FHitResult 값
	 * @param DamageTags 피해를 받은 이유가 되는 태그 (컨테이너)
	 * @param InstigatorCharacter 피해의 가해자 캐릭터 (수류탄에 의해 피해를 입었다면, 수류탄을 던진 캐릭터)
	 * @param DamageCauser 피해를 실제로 준 액터 (수류탄에 의해 피해를 입었다면, 수류탄)
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "CharacterBase|Attribute|Damage")
	void OnDamaged(float DamageAmount, const FHitResult& HitResult, const FGameplayTagContainer& DamageTags,
		ACharacterBase* InstigatorCharacter, AActor* DamageCauser);

	/**
	 * @brief 이동 속도 어트리뷰트가 변경되었을 때 호출됩니다.
	 */
	void OnMoveSpeedAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase|Attribute")
	void ForceInitializeStats();



	/**
	 * @brief 공격 시 대상에게 추가로 적용시킬 이펙트 클래스입니다.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enemy|Attack")
	TSubclassOf<UGameplayEffect> AdditionalAttackEffectOnTarget;

	UPROPERTY(BlueprintReadOnly)
	class UAbilitySystemCompBase* AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly)
	class UAttributeSetBase* AttributeSetBase;

	FGameplayTag DeadTag;

	/** @brief 명시적으로 사망했음을 의미 */
	bool bExplicitlyDead;

	/**
	 * @brief 사망 시 제거될 이펙트의 태그
	 * @details 기본적으로 Effect.RemoveOnDeath 입니다.
	 * @see ACharacterBase() 참고.
	 */
	FGameplayTag EffectRemoveOnDeathTag;

	

	/**
	 * @brief 시작 효과, 즉 패시브 효과입니다.
	 * @see AddStartupEffects() 참고.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "CharacterBase|Attributes")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	/**
	 * @brief 캐릭터가 기본으로 가지는 어빌리티입니다.
	 * @see AddAbilities() 참고.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterBase|Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> Abilities;

	/**
	 * @brief 캐릭터 사망 시 재생될 몽타주입니다.
	 * @see HandleDie() 참고.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterBase|Animations")
	UAnimMontage* DeathMontage;

	/**
	 * @brief DeathMontage에서 사용하고 싶은 몽타주 섹션입니다.
	 * @details 딱히 설정할 필요는 없습니다.
	 * @see HandleDie() 참고.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterBase|Animations")
	FName DeathMontageStartSectionName;

	FDelegateHandle MoveSpeedChangedDelegateHandle;



	/**
	* @brief 장착한 무기
	* @see UnEquipWeapon() 및 EquipWeapon() 참고.
	*/
	UPROPERTY()
	class UItem_WeaponBase* EquippedWeapon;

	/**
	* @brief 장착한 방어구
	* @see UnEquipArmor() 및 EquipArmor() 참고.
	*/
	UPROPERTY()
	class UItem_ArmorBase* EquippedArmor;

	/** @brief 기본 어트리뷰트가 초기화되었는지를 의미 */
	bool bDefaultAttributeInitialized;



	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UAIPerceptionStimuliSourceComponent* AIPerceptionComponent;
};
