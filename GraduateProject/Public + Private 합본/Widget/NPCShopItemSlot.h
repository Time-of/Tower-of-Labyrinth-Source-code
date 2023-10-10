
#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include <GraduateProject/GraduateProject.h>

#include "NPCShopItemSlot.generated.h"

class UItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMouseEnterDelegate, UItemBase*, ItemHas, EShopInteractType, ShopInteractType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseLeaveDelegate, UItemBase*, ItemHas);



/**
 * @author 이성수
 * @brief 상점 NPC UI의 아이템 슬롯 위젯 클래스입니다.
 * @since 2023-02-26
 */
UCLASS()
class GRADUATEPROJECT_API UNPCShopItemSlot : public UUserWidget
{
	GENERATED_BODY()
		
public:
	UNPCShopItemSlot(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	/**
	* @brief 새 상호작용 스트래티지(전략)을 설정합니다.
	* @param NewStrategy 구매/판매/제작 전략 클래스
	*/
	void SetInteractStrategy(class ShopSlotInteractStrategy* NewStrategy);

	class ShopSlotInteractStrategy* GetInteractStrategy() const
	{
		return InteractStrategy;
	}

	/**
	* @brief 슬롯에 아이템 정보를 업데이트합니다.
	* @param Item 아이템 정보
	* @param NewCount 새롭게 설정할 아이템 수량
	*/
	UFUNCTION(BlueprintCallable, Category = "ItemSlot")
	void UpdateItemSlot(class UItemBase* Item, int32 NewItemCount);

	UFUNCTION()
	void OnInventoryUpdatedFromAssetId(FPrimaryAssetId Item, int32 NewItemCount);

	/** @brief 인벤토리가 업데이트 되었을 때 슬롯을 업데이트하기 위해 콜백으로 쓰이는 함수입니다. */
	UFUNCTION()
	void OnInventoryUpdated(class UItemBase* Item, int32 NewItemCount);

	/**
	* @brief 현재 InteractType에 따라서 아이템에 상호작용(구매/판매/제작)을 시도합니다.
	* @details ItemCount가 1 이상이어야 합니다. (0 이하인 경우 반드시 false)
	* @return 성공 시 true, 실패 시 false
	* @param ConfirmWidget 확인 UI
	* @param InteractItemCount 상호작용(구매/판매/제작)할 아이템 개수
	*/
	UFUNCTION()
	void InteractItem(class UNPCShopConfirmInteract* ConfirmWidget, int32 InteractItemCount = 1);



	/**
	* @brief 버튼이 눌렸을 때 확인 위젯을 만들고 보여줍니다.
	* @see UpdateItemSlot()에서 바인딩을 진행합니다.
	*/
	UFUNCTION()
	void ShowConfirmWidgetOnButtonClicked();



	UPROPERTY(BlueprintAssignable, Category = "ItemSlot")
	FOnMouseEnterDelegate MouseEnterDelegate;

	UPROPERTY(BlueprintAssignable, Category = "ItemSlot")
	FOnMouseLeaveDelegate MouseLeaveDelegate;



	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "ItemSlot")
	EShopInteractType InteractType;

	UPROPERTY(BlueprintReadOnly, Category = "ItemSlot")
	UItemBase* ItemHas;

	UPROPERTY(BlueprintReadOnly, Category = "ItemSlot")
	int32 ItemCount;



	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
	class UButton* ItemButton;



protected:
	void SetItemImage();



	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
	class UImage* ItemImage;

	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
	class UTextBlock* ItemCountText;

	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
	class UOverlay* ItemCountOverlay;

	UPROPERTY(BlueprintReadWrite, Category = "ItemSlot", meta = (BindWidget))
	class UBorder* LockBorder;

	UPROPERTY()
	TSubclassOf<class UNPCShopConfirmInteract> ConfirmWidgetClass;

	/** @brief 상호작용 스트래티지 */
	class ShopSlotInteractStrategy* InteractStrategy;
};
