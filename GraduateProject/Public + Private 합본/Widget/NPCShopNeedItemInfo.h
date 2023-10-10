

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCShopNeedItemInfo.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseEnterToNeedItemInfo, UItemBase*, ItemHas);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseLeaveFromNeedItemInfo, UItemBase*, ItemHas);



/**
 * @author 이성수
 * @brief 상점 NPC와 상호작용(구매/제작)할 때 필요한 아이템에 대한 정보를 담은 위젯 클래스입니다.
 * @since 2023-02-28
 */
UCLASS()
class GRADUATEPROJECT_API UNPCShopNeedItemInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UNPCShopNeedItemInfo(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "NeedItemInfo")
	void UpdateInfo(class UItemBase* Item, int32 NeedItemQuantity);

	void UpdateNeedQuantity(int32 NewNeedQuantity);

	/** @brief bool 파라미터 값에 따라 수량 텍스트의 색상을 빨간색 또는 하얀색으로 변경합니다. */
	void SetQuantityTextColorToRed(bool bChangeToRed);

	class UItemBase* GetDisplayedItem() const;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;



	UPROPERTY(BlueprintAssignable, Category = "NeedItemInfo")
	FOnMouseEnterToNeedItemInfo MouseEnterDelegate;

	UPROPERTY(BlueprintAssignable, Category = "NeedItemInfo")
	FOnMouseLeaveFromNeedItemInfo MouseLeaveDelegate;



	/** @brief 필요 수량 */
	int32 NeedQuantity;



protected:
	UPROPERTY()
	class UItemBase* DisplayedItem;

	UPROPERTY(BlueprintReadWrite, Category = "NeedItemInfo", meta = (BindWidget))
	class UImage* Image_ItemIcon;

	UPROPERTY(BlueprintReadWrite, Category = "NeedItemInfo", meta = (BindWidget))
	class UTextBlock* ItemQuantityText;
};
