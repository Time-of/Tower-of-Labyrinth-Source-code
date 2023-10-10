
#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include <GraduateProject/GraduateProject.h>

#include "InventorySkillSlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseEnterSkillSlotDelegate, UItem_CharacterSkillBase*, SkillHas);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseLeaveSkillSlotDelegate, UItem_CharacterSkillBase*, SkillHas);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillButtonClickedDelegate, EEnemyAbilityID, AbilityID);



/**
 * @author 이성수
 * @brief 인벤토리에 사용되는 스킬 슬롯 위젯 클래스입니다.
 * @since 2023-03-03
 */
UCLASS()
class GRADUATEPROJECT_API UInventorySkillSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventorySkillSlotWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void UpdateSlot(class UItem_CharacterSkillBase* SkillItem);

	FORCEINLINE void UpdateSkillKeyText(const FString& TextToUpdate);

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;



	UPROPERTY(BlueprintAssignable, Category = "Inventory|Skill")
	FOnMouseEnterSkillSlotDelegate OnMouseEnteredToSlot;

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Skill")
	FOnMouseLeaveSkillSlotDelegate OnMouseLeaveFromSlot;

	

	EEnemyAbilityID AbilityID;

	FOnSkillButtonClickedDelegate OnSkillButtonClicked;



protected:
	UFUNCTION()
	void InternalOnSkillButtonClicked();



	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Skill", meta = (BindWidget))
	class UButton* SkillButton;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Skill", meta = (BindWidget))
	class UImage* SkillImage;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Skill", meta = (BindWidget))
	class UTextBlock* SkillKeyText;

	UPROPERTY()
	class UItem_CharacterSkillBase* SkillHas;
};
