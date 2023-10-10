
#pragma once

#include "CoreMinimal.h"
#include "Widget/InteractableNPCWidgetBase.h"
#include "CombineWeaponWidget.generated.h"



/**
 * @author 이성수
 * @brief 무기 조합 위젯 클래스입니다.
 * @since 2023-04-27
 */
UCLASS()
class GRADUATEPROJECT_API UCombineWeaponWidget : public UInteractableNPCWidgetBase
{
	GENERATED_BODY()

public:
	UCombineWeaponWidget(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() override;

	void NativeDestruct() override;



protected:
	void CreateInventorySlots(uint8 Row, uint8 Column);

	void RemoveInventorySlots();

	void LoadWeaponsFromInventory();

	void SetInventorySlotByIndex(class UItemBase* ItemToSet, int32 NewItemCount, int32 Index);

	UFUNCTION()
	void OnInventoryUpdated(class UItemBase* Item, int32 NewItemCount, int32 DeltaQuantity);

	void UpdateSlotOnItemRemoved(class UItemBase* Item, int32& ItemQuantity, int32 ItemMaxQuantity);

	void UpdateSlotOnItemAdded(uint32& SlotIndex, class UItemBase* Item, int32& ItemQuantity, int32 ItemMaxQuantity);

	UFUNCTION()
	void ShowItemDetailsWidget(class UItemBase* Item);

	UFUNCTION()
	void HideItemDetailsWidget(class UItemBase* Item);

	UFUNCTION()
	void SelectInventoryItem(class UInventorySlot* InvSlot);

	bool GetEmptyMaterialSlot(bool& OutIsLeftSlot);

	void SetMaterialSlot(bool bIsLeftSlot, class UInventorySlot* InvSlot);

	void SetMaterialSlotToEmpty(bool bIsLeftSlot);

	/**
	* @brief 인벤토리 슬롯의 하이라이트를 변경합니다. (기존 슬롯 하이라이트 해제, 새 슬롯 하이라이트 활성)
	* @param bIsLeftSlot true라면 왼쪽 재료 슬롯에 해당하는 하이라이트, false라면 오른쪽 재료 슬롯.
	* @param InvSlot 새 하이라이트가 될 인벤토리 슬롯 위젯
	*/
	void ChangeHighlightOnInventorySlot(bool bIsLeftSlot, UInventorySlot* InvSlot);

	UFUNCTION()
	void InternalSetLeftMaterialSlotToEmpty(class UInventorySlot* InvSlot);

	UFUNCTION()
	void InternalSetRightMaterialSlotToEmpty(class UInventorySlot* InvSlot);

	UFUNCTION()
	void ExecuteCombineOnCombineButtonClicked();

	void AddRandomWeaponToResultSlot(float MinWeight, float MaxWeight);

	UFUNCTION()
	void OnResultSlotClicked(class UInventorySlot* InvSlot);

	void BindCombineDelegates();

	void UnbindCombineDelegates();

	void BindMouseEnterLeaveDelegatesOnSlot(class UInventorySlot* InvSlot);

	void UnbindMouseEnterLeaveDelegatesOnSlot(class UInventorySlot* InvSlot);

	/** @brief 인벤토리 인덱스가 슬롯 수를 초과한 경우 새 슬롯 할당하기 */
	void CreateNewSlotIfOutOfIndex();

	UFUNCTION(BlueprintImplementableEvent)
	void OnResultSlotItemAdded();



	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	uint8 DefaultInventoryRowCount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	uint8 DefaultInventoryColumnCount;

	uint8 CurrentInventoryRowCount;



	UPROPERTY(BlueprintReadWrite, Category = "CombineWeaponWidget", meta = (BindWidget))
	class UButton* CombineButton;

	UPROPERTY(BlueprintReadWrite, Category = "CombineWeaponWidget", meta = (BindWidget))
	class UInventorySlot* CombineResultItemSlot;

	UPROPERTY(BlueprintReadWrite, Category = "CombineWeaponWidget", meta = (BindWidget))
	class UInventorySlot* CombineMaterialItemSlot_Left;

	UPROPERTY(BlueprintReadWrite, Category = "CombineWeaponWidget", meta = (BindWidget))
	class UInventorySlot* CombineMaterialItemSlot_Right;



	UPROPERTY(BlueprintReadWrite, Category = "CombineWeaponWidget", meta = (BindWidget))
	class UUniformGridPanel* UniformGrid_Inventory;

	UPROPERTY(BlueprintReadWrite, Category = "CombineWeaponWidget", meta = (BindWidget))
	class UNPCShopItemDetails_Simplified* ItemDetailsWidget;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CombineWeaponWidget")
	TSubclassOf<class UInventorySlot> InventorySlotClass;

	uint32 InventoryLastIndex;

	UPROPERTY(VisibleAnywhere, Category = "CombineWeaponWidget")
	class UInventorySlot* SelectedLeftSlotRef;

	UPROPERTY(VisibleAnywhere, Category = "CombineWeaponWidget")
	class UInventorySlot* SelectedRightSlotRef;
};
