
#pragma once

#include "CoreMinimal.h"

#include "Item/ItemBase.h"
#include <GraduateProject/GraduateProject.h>

#include "Item_CharacterSkillBase.generated.h"



/**
 * @author 이성수
 * @brief 재료 아이템 베이스 클래스
 * @since 2023-02-23
 */
UCLASS()
class GRADUATEPROJECT_API UItem_CharacterSkillBase : public UItemBase
{
	GENERATED_BODY()

public:
	UItem_CharacterSkillBase();

	bool TryEquipItem() override;

	UFUNCTION(BlueprintCallable)
	void EquipSkillItem(EEnemyAbilityID SkillID);



	/** @brief 부여될 어빌리티 클래스 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item|Ability")
	TSubclassOf<class UGameplayAbilityBase> GrantedAbilityClass;

	/** @brief 부여될 어빌리티의 레벨 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item|Ability")
	int32 GrantedAbilityLevel;
};
