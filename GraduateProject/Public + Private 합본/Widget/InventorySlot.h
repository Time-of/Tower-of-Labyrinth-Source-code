
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class UItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseEnterInvSlotDelegate, UItemBase*, ItemHas);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseLeaveInvSlotDelegate, UItemBase*, ItemHas);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedInvSlotDelegate, UInventorySlot*, InvSlot);



/**
 * @author 이성수
 * @brief 인벤토리 슬롯 위젯 클래스입니다.
 * @since 2023-03-03
 */
UCLASS()
class GRADUATEPROJECT_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventorySlot(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Slot")
	void ShowHighlight(bool bShow = true);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Slot")
	void ShowEquipText(bool bShow = true);

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	/**
	* @brief 슬롯에 아이템 정보를 업데이트합니다.
	* @param Item 아이템 정보
	* @param NewCount 새롭게 설정할 아이템 수량
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory|Slot")
	void UpdateItemSlot(class UItemBase* Item, int32 NewItemCount);

	void UpdateItemQuantity(int32 NewQuantity);

	/**
	* @brief 현재 EquipText가 보여지고 있는지 여부를 가져옵니다.
	* @return EquipText가 Collapsed가 아니라면 true
	*/
	bool IsEquipTextVisible() const;

	/** @brief 하이라이트가 켜져있는지 여부를 반환 */
	bool IsHighlighted();

	

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Slot")
	FOnMouseEnterInvSlotDelegate MouseEnterDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Slot")
	FOnMouseLeaveInvSlotDelegate MouseLeaveDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Slot")
	FOnClickedInvSlotDelegate OnClickedDelegate;



	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Slot")
	UItemBase* ItemHas;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Slot")
	int32 ItemCount;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Slot", meta = (BindWidget))
	class UButton* ItemButton;



protected:
	void RefreshBrush();



	/** @brief 클릭 시 하이라이트될 보더 */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Slot", meta = (BindWidget))
	class UBorder* HighlightBorder;

	/** @brief 장착 여부를 알려주는 텍스트 */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Slot", meta = (BindWidget))
	class UTextBlock* EquipText;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Slot", meta = (BindWidget))
	class UImage* ItemImage;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Slot", meta = (BindWidget))
	class UTextBlock* ItemCountText;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Slot", meta = (BindWidget))
	class UOverlay* ItemCountOverlay;



private:
	UFUNCTION()
	void BroadcastOnSlotClicked();
};
