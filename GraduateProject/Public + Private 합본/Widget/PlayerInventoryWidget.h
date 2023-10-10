
#pragma once

#include "CoreMinimal.h"

#include "Widget/InteractableNPCWidgetBase.h"

#include <GraduateProject/GraduateProject.h>

#include "PlayerInventoryWidget.generated.h"



/**
 * @author 이성수
 * @brief 플레이어 인벤토리 위젯 클래스입니다.
 * @since 2023-03-03
 */
UCLASS()
class GRADUATEPROJECT_API UPlayerInventoryWidget : public UInteractableNPCWidgetBase
{
	GENERATED_BODY()
	
public:
	UPlayerInventoryWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	/** @brief 추가로 Inventory 입력을 받아도 닫히도록 오버라이드 */
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddThisToViewport(class UCameraComponent* OriginalCamera, class UCameraComponent* ViewCamera, AActor* PlayerCharacter);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetupItemSlots(class UUniformGridPanel* UniformGrid, TSubclassOf<class UInventorySlot> InventorySlotClass);

	void RemoveItemSlots(UUniformGridPanel* UniformGrid);

	void ClearSkillSlots();

	void ClearSkillSlot(class UInventorySkillSlotWidget* SkillSlot);

	virtual void RemoveThisFromParent() override;

	void LoadInventoryData();

	//void ForceReloadSkillSlots();

	void LoadDefaultSkillSlots();

	void SetupDefaultSkillSlot(class UInventorySkillSlotWidget* SkillSlotWidget, EEnemyAbilityID AbilityID);

	UFUNCTION()
	void UpdateSkillSlot(class UItem_CharacterSkillBase* SkillItem, EEnemyAbilityID AbilityID);



protected:
	UFUNCTION()
	void ShowItemDetailsWidget(class UItemBase* Item);

	UFUNCTION()
	void HideItemDetailsWidget(class UItemBase* Item);

	UFUNCTION()
	void ShowSkillDetailsWidget(class UItem_CharacterSkillBase* SkillItem);

	UFUNCTION()
	void HideSkillDetailsWidget(class UItem_CharacterSkillBase* SkillItem);

	void UpdateStatTexts();

	UFUNCTION()
	void UpdateHealthText(float NewHealth);

	void UpdateEquipButtonInfo();

	UFUNCTION()
	void OnEquipButtonClicked();

	UFUNCTION()
	void OnInventoryUpdated(class UItemBase* Item, int32 NewItemCount, int32 DeltaQuantity);

	/** @breif 인벤토리 슬롯을 선택하여 SelectedSlotRef에 저장합니다. */
	UFUNCTION()
	void SelectSlot(class UInventorySlot* SelectedInvSlot);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetItemSlotByIndex(UUniformGridPanel* UniformGrid, UItemBase* ItemToSet, int32 NewItemCount, int32 Index);

	/**
	* @brief 아이템을 현재 개수와 최대 개수에 따라 순차적으로 설정하고자 할 때 사용합니다.
	* @since 2023-04-19
	* @see LoadInventoryData() 및 OnInventoryUpdated() 참고.
	*/
	void CustomSetItemByQuantity(int32& ItemQuantity, int32 ItemMaxQuantity, class UItemBase* Item, TFunction<void()> WeaponFunc, TFunction<void()> ArmorFunc, TFunction<void()> MiscFunc);

	void SetItemByType(class UItemBase* Item, int32 ItemQuantity = 0);

	void UpdateSlotOnInventoryChanged(class UUniformGridPanel* UniformGridWidget, int32& SlotIndex, class UItemBase* Item, int32& ItemQuantity, int32 ItemMaxQuantity);

	UFUNCTION()
	void UpdateMoneyTextOnMoneyChanged(float NewMoney);



	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory")
	int32 InventorySlotRow;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory")
	int32 InventorySlotColumn;

	UPROPERTY()
	class UCameraComponent* OriginalCameraRef;

	UPROPERTY()
	class UCameraComponent* ViewCameraRef;



	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UTextBlock* HpText;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UTextBlock* StrText;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UTextBlock* DefText;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UImage* EquippedArmorItemImage;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UUniformGridPanel* UniformGrid_Weapon;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UUniformGridPanel* UniformGrid_Armor;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UUniformGridPanel* UniformGrid_Misc;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UButton* Button_EquipItem;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UTextBlock* EquipItemButtonText;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UNPCShopItemDetails* ItemDetailsWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UInventorySkillDetailsWidget* SkillDetailsWidget;

	/** @brief 돈(골드) 보유량 위젯 */
	UPROPERTY(BlueprintReadWrite, Category = "ItemDetails", meta = (BindWidget))
	class UNPCShopNeedItemInfo* NeedInfo_Money;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UInventorySkillSlotWidget* SkillSlot_Q;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UInventorySkillSlotWidget* SkillSlot_E;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UInventorySkillSlotWidget* SkillSlot_F;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	class UInventorySkillSlotWidget* SkillSlot_RM;



	/** @brief 선택된 슬롯의 레퍼런스 */
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	class UInventorySlot* SelectedSlotRef;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	class UInventorySlot* EquippedWeaponSlotRef;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	class UInventorySlot* EquippedArmorSlotRef;



	UPROPERTY()
	TSubclassOf<class UInventorySkillSlotWidget> SkillSlotWidgetClass;



	int32 WeaponSlotIndex;
	int32 ArmorSlotIndex;
	int32 MiscSlotIndex;
};
