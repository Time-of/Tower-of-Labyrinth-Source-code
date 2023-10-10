
#include "Widget/NPCShopNeedItemInfo.h"

#include "Item/ItemBase.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"



UNPCShopNeedItemInfo::UNPCShopNeedItemInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	NeedQuantity(-1),
	DisplayedItem(nullptr),
	Image_ItemIcon(nullptr), ItemQuantityText(nullptr)
{

}



void UNPCShopNeedItemInfo::UpdateInfo(UItemBase* Item, int32 NeedItemQuantity)
{
	// 유효하지 않은 경우 뷰포트에서 제거
	if (!IsValid(Item) || NeedItemQuantity < 0)
	{
		RemoveFromParent();

		return;
	}

	DisplayedItem = Item;

	// 아이콘 업데이트
	FSlateBrush NewBrush = DisplayedItem->ItemIcon;
	NewBrush.ImageSize = FVector2D(128.0f, 128.0f);
	Image_ItemIcon->SetBrush(NewBrush);

	UpdateNeedQuantity(NeedItemQuantity);
}



void UNPCShopNeedItemInfo::UpdateNeedQuantity(int32 NewNeedQuantity)
{
	NeedQuantity = NewNeedQuantity;
	ItemQuantityText->SetText(FText::AsNumber(NeedQuantity));
}



void UNPCShopNeedItemInfo::SetQuantityTextColorToRed(bool bChangeToRed)
{
	ItemQuantityText->SetColorAndOpacity(bChangeToRed ?
		FSlateColor(FLinearColor::Red) : FSlateColor(FLinearColor::White));
}



UItemBase* UNPCShopNeedItemInfo::GetDisplayedItem() const
{
	return DisplayedItem;
}



void UNPCShopNeedItemInfo::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	MouseEnterDelegate.Broadcast(DisplayedItem);
}



void UNPCShopNeedItemInfo::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	MouseLeaveDelegate.Broadcast(DisplayedItem);
}
