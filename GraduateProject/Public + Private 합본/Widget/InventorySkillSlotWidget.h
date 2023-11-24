
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
	void UpdateSlot(class UItem_CharacterSkillBase* SkillItem, bool bIsHUD = false);

	/** @brief 스킬 단축키 텍스트 업데이트 */
	void UpdateSkillKeyText(const FString& TextToUpdate);

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	/**
	* @brief 스킬 코스트에 따라 버튼 활성화 여부를 새로고침하는 함수.
	* @details PlayerSkillHudWidget에서 스킬 사용 여부에 따라 사용 가능한 스킬을 직관적으로 표시하기 위해 사용
	*/
	void RefreshButtonActivationBySkillCost(float CurrentPlayerSkillMana);

	/**
	* @brief 스킬 마나 float 변수 새로고침.
	* @details HUD인 경우 UpdateSlot()에서 호출, 스킬 마나 사용량 배수가 변경되면 HUD에서 수동으로 호출
	*/
	void RefreshSkillManaUsage(float NewSkillManaUsageRate);

	void UpdateSkillManaUsageText();



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

	/** @brief 스킬 발동을 위해 필요한 스킬 마나 량. HUD에서만 사용 */
	UPROPERTY()
	float SkillManaUsage;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Skill", meta = (BindWidget))
	class UTextBlock* SkillManaUsageText;
};