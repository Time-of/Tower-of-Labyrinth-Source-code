
#include "Item/ItemBase.h"

#include "Blueprint/WidgetBlueprintLibrary.h"



UItemBase::UItemBase() : Super(),
	ItemPrice(0.0f), MaxCount(1)
{
	
}



FPrimaryAssetId UItemBase::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}



bool UItemBase::TryEquipItem()
{
	return false;
}



UTexture2D* UItemBase::GetIcon_Implementation() const
{
	return UWidgetBlueprintLibrary::GetBrushResourceAsTexture2D(ItemIcon);
}
