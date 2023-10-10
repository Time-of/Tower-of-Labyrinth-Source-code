
#include "Widget/InventorySlot.h"

#include "Item/ItemBase.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"



UInventorySlot::UInventorySlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	ItemHas(nullptr), ItemCount(0), ItemButton(nullptr),
	HighlightBorder(nullptr), EquipText(nullptr),
	ItemImage(nullptr), ItemCountText(nullptr), ItemCountOverlay(nullptr)
{

}



void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateItemSlot(nullptr, 0);

	ItemButton->OnClicked.AddUniqueDynamic(this, &UInventorySlot::BroadcastOnSlotClicked);
}



void UInventorySlot::ShowHighlight(bool bShow)
{
	HighlightBorder->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}



void UInventorySlot::ShowEquipText(bool bShow)
{
	EquipText->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}



void UInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	MouseEnterDelegate.Broadcast(ItemHas);
}



void UInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	MouseLeaveDelegate.Broadcast(ItemHas);
}



void UInventorySlot::UpdateItemSlot(UItemBase* Item, int32 NewItemCount)
{
	ItemHas = Item;
	UpdateItemQuantity(NewItemCount);
	RefreshBrush();
}



void UInventorySlot::UpdateItemQuantity(int32 NewQuantity)
{
	ItemCount = NewQuantity;
	ItemCountText->SetText(FText::AsNumber(ItemCount));
	ItemCountOverlay->SetVisibility(ItemCount > 1 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}



bool UInventorySlot::IsEquipTextVisible() const
{
	return EquipText->GetVisibility() != ESlateVisibility::Collapsed;
}



bool UInventorySlot::IsHighlighted()
{
	return HighlightBorder->GetVisibility() == ESlateVisibility::SelfHitTestInvisible;
}



void UInventorySlot::RefreshBrush()
{
	FSlateBrush NewBrush;
	if (!IsValid(ItemHas))
	{
		NewBrush = FSlateBrush();
		NewBrush.SetResourceObject(nullptr);
		NewBrush.TintColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		NewBrush = ItemHas->ItemIcon;

		// 아이템이 0개 이하라면 반투명화
		FLinearColor Tint = NewBrush.TintColor.GetSpecifiedColor();
		Tint.A = ItemCount > 0 ? 1.0f : 0.5f;
		NewBrush.TintColor = Tint;
		ItemButton->SetIsEnabled(ItemCount > 0);
	}

	NewBrush.ImageSize = FVector2D(128.0f, 128.0f);

	ItemImage->SetBrush(NewBrush);
}



void UInventorySlot::BroadcastOnSlotClicked()
{
	OnClickedDelegate.Broadcast(this);

	UE_LOG(LogTemp, Log, TEXT("OnClickedDelegate 브로드캐스트"));
}
