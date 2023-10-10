
#pragma once

#include "CoreMinimal.h"

#include "Widget/InteractableNPCWidgetBase.h"

#include <GraduateProject/GraduateProject.h>

#include "AssignSkillWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAssignSuccessedDelegate);



/**
 * @author 이성수
 * @brief 스킬 할당 위젯 클래스입니다.
 * @since 2023-04-25
 */
UCLASS()
class GRADUATEPROJECT_API UAssignSkillWidget : public UInteractableNPCWidgetBase
{
	GENERATED_BODY()

public:
	UAssignSkillWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;



	UPROPERTY(BlueprintReadOnly, Category = "AssignSkill")
	class UItem_CharacterSkillBase* SkillItemToAssign;

	UPROPERTY(BlueprintAssignable, Category = "AssignSkill")
	FOnAssignSuccessedDelegate OnAssignSuccessed;



protected:
	void LoadDefaultSkillSlots();

	void SetupDefaultSkillSlot(class UInventorySkillSlotWidget* SkillSlotWidget, EEnemyAbilityID AbilityID);

	void UpdateSkillSlot(class UItem_CharacterSkillBase* SkillItem, EEnemyAbilityID AbilityID);

	/** @brief 버튼을 클릭했을 때 해당 칸에 스킬 부여 */
	UFUNCTION()
	void AssignSkillOnClicked(EEnemyAbilityID AbilityID);



	UPROPERTY(BlueprintReadWrite, Category = "AssignSkill", meta = (BindWidget))
	class UInventorySkillSlotWidget* SkillSlot_Q;

	UPROPERTY(BlueprintReadWrite, Category = "AssignSkill", meta = (BindWidget))
	class UInventorySkillSlotWidget* SkillSlot_E;

	UPROPERTY(BlueprintReadWrite, Category = "AssignSkill", meta = (BindWidget))
	class UInventorySkillSlotWidget* SkillSlot_F;
};
