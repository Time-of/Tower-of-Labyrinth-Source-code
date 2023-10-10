
#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include <GraduateProject/GraduateProject.h>

#include "PlayerSkillHudWidget.generated.h"



/**
 * @author 이성수
 * @brief 플레이어 스킬 HUD 위젯 클래스입니다.
 * @since 2023-05-12
 */
UCLASS()
class GRADUATEPROJECT_API UPlayerSkillHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerSkillHudWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;



protected:
	void LoadDefaultSkillSlots();

	void SetupDefaultSkillSlot(class UInventorySkillSlotWidget* SkillSlotWidget, EEnemyAbilityID AbilityID);

	UFUNCTION()
	void UpdateSkillSlot(class UItem_CharacterSkillBase* SkillItem, EEnemyAbilityID AbilityID);



	UPROPERTY(BlueprintReadWrite, Category = "PlayerSkillHud", meta = (BindWidget))
	class UInventorySkillSlotWidget* SkillSlot_Q;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerSkillHud", meta = (BindWidget))
	class UInventorySkillSlotWidget* SkillSlot_E;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerSkillHud", meta = (BindWidget))
	class UInventorySkillSlotWidget* SkillSlot_F;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerSkillHud", meta = (BindWidget))
	class UInventorySkillSlotWidget* SkillSlot_RMB;
};
