
#pragma once

#include "CoreMinimal.h"

#include "CharacterBase.h"
#include <GraduateProject/GraduateProject.h>

#include "PlayerCharacterBase.generated.h"

class UItem_CharacterSkillBase;



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedDelegate, APlayerCharacterBase*, Player);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillEquipmentChangedDelegate, UItem_CharacterSkillBase*, SkillItem, EEnemyAbilityID, AbilityID);



/**
 * @author 이성수
 * @brief 플레이어 캐릭터 베이스입니다. 어빌리티 시스템 인풋 바인딩 등 플레이어에게만 존재하는 기능을 위해 존재합니다.
 * @since 2023-02-26
 */
UCLASS()
class GRADUATEPROJECT_API APlayerCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacterBase();

	virtual void PostInitProperties() override;

	/**
	* @brief 스킬 게이지(SkillMana)가 변경되었을 때 호출
	* @param DeltaSkillMana 스킬 게이지(SkillMana)의 변화율
	* @param SourceTags 원인 태그
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "CharacterBase|Attribute")
	void OnSkillManaChanged(float DeltaSkillMana, const FGameplayTagContainer& SourceTags);

	/**
	* @brief 최대 스킬 게이지(MaxSkillMana)가 변경되었을 때 호출
	* @param DeltaMaxSkillMana 최대 스킬 게이지(MaxSkillMana)의 변화율
	* @param SourceTags 원인 태그
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "CharacterBase|Attribute")
	void OnMaxSkillManaChanged(float DeltaMaxSkillMana, const FGameplayTagContainer& SourceTags);

	/**
	* @brief 현재 스킬 게이지(SkillMana)를 가져옵니다.
	* @return SkillMana 어트리뷰트 값, 어트리뷰트 접근 실패 시 -1.0f
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|State|Attributes")
	float GetSkillMana() const;

	/**
	* @brief 현재 최대 스킬 게이지(MaxSkillMana)를 가져옵니다.
	* @return MaxSkillMana 어트리뷰트 값, 어트리뷰트 접근 실패 시 -1.0f
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|State|Attributes")
	float GetMaxSkillMana() const;

	/** @brief 임시 데이터를 GISS에 저장 */
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|TemporaryData")
	void SaveTemporaryData();

	/** @brief 임시 데이터를 GISS로부터 불러오기, BeginPlay() 및 GISS_TemporaryPlayerData에서 수행 */
	void LoadTemporaryData();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase|Skill")
	bool HasSkillItem(class UItem_CharacterSkillBase* SkillItem) const;

	UFUNCTION(BlueprintCallable, Category = "CharacterBase|Skill")
	TMap<EEnemyAbilityID, class UItem_CharacterSkillBase*> GetEquippedSkills() const;

	/** @brief 스킬 아이템을 장착합니다. */
	virtual void EquipSkillItem(class UItem_CharacterSkillBase* SkillItemToEquip, EEnemyAbilityID AbilityID);

	/** @brief 스킬 아이템을 장착 해제합니다. */
	virtual void UnEquipSkillItem(EEnemyAbilityID AbilityID);



	FOnPlayerDiedDelegate OnPlayerDied;

	FOnSkillEquipmentChangedDelegate OnSkillEquipmentChanged;
	


protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** @brief 어빌리티 시스템의 인풋을 바인드합니다. */
	void BindAbilitySystemInputComponent(UInputComponent* PlayerInputComponent);

	/** @brief 장착된 아이템의 수량을 감시하여, 장착된 아이템의 수량이 0 이하가 되면 장착해제시킵니다. */
	UFUNCTION()
	void ObserveEquippedItemsQuantity(class UItemBase* Item, int32 NewQuantity, int32 DeltaQuantity);

	void InitializeDefaultAttribute() override;

	

	void EquipWeapon(class UItem_WeaponBase* WeaponToEquip) override;

	void UnEquipWeapon() override;

	virtual void DieAction() override;

	//UFUNCTION()
	//void OnBuffTagChange(const FGameplayTag Tag, int32 NewCount);



	/** @brief 어빌리티 시스템 인풋이 바인드 되었는가 여부 */
	bool bIsASCInputBound;

	/**
	 * @brief 기본 어트리뷰트, 즉 스탯입니다.
	 * @see InitializeDefaultAttribute() 참고.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "CharacterBase|Attributes")
	TSubclassOf<class UGameplayEffect> DefaultAttribute;



	UPROPERTY(VisibleAnywhere)
	TMap<EEnemyAbilityID, class UItem_CharacterSkillBase*> EquippedSkills;
};
