
#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "Item_WeaponBase.generated.h"



/**
 * @author 이성수
 * @brief 무기 아이템 베이스 클래스
 * @since 2023-02-23
 */
UCLASS()
class GRADUATEPROJECT_API UItem_WeaponBase : public UItemBase
{
	GENERATED_BODY()

public:
	UItem_WeaponBase();

	bool TryEquipItem() override;



	/** @brief 새롭게 부여할 공격 시 추가효과 부여 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item|Weapon|Effect")
	TSubclassOf<class UGameplayEffect> AdditionalEffectOnAttackClass;

	/** @brief 무기 스태틱 메쉬 액터 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item|Weapon|Mesh")
	TSubclassOf<class AEquipSMActor> WeaponMesh;

	/** @brief 장착 시 부여되는 스킬 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item|Weapon|GrantSkill")
	class UItem_CharacterSkillBase* GrantedSkillOnEquip;

	/** @brief 장착 즉시 스킬 사용 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item|Weapon|GrantSkill")
	bool bUseSkillImmediately;
};
