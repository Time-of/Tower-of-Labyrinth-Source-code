
#include "Widget/NPCShopCraftSlot.h"

#include "GameAssetManager.h"
#include "GameInstance/GISS_InteractableNpcData.h"
#include "GameInstance/GISS_PlayerInventory.h"
#include "GameInstance/GISS_PlayerMoney.h"
#include "Item/ItemBase.h"
#include "Widget/NPCShopConfirmInteract.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"



UNPCShopCraftSlot::UNPCShopCraftSlot(const FObjectInitializer& ObjectInitializer) : UNPCShopItemSlot(ObjectInitializer)
{

}



void UNPCShopCraftSlot::SetNeedItems(const TArray<FCraftNeedItem>& NewNeedItems)
{
	NeedItems = NewNeedItems;
}



TArray<FCraftNeedItem> UNPCShopCraftSlot::GetNeedItems() const
{
	return NeedItems;
}



/*
bool UNPCShopCraftSlot::CraftItem(int32 CraftCount)
{
	if (ItemCount <= 0 || CraftCount > ItemCount || !IsValid(ItemHas)) return false;

	UGISS_PlayerInventory* InventorySS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();
	UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();



	// 재료를 모두 가지고 있는지 조건 검사
	for (const FCraftNeedItem& NeedItem : NeedItems)
	{
		UItemBase* LoadedItem = AssetManager.LoadItem(NeedItem.NeedItem);

		// 아이템이 유효하지 않거나, 아이템을 보유한 적이 없거나, 보유량이 필요량보다 적다면 가지고 있지 않은 것으로 간주
		if (!IsValid(LoadedItem) || !InventorySS->InventoryMap.Contains(LoadedItem) || InventorySS->InventoryMap[LoadedItem].Quantity < NeedItem.NeedQuantity)
		{
			return false;
		}
	}

	// 돈을 소모
	if (!GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>()->TryUseMoney(CraftCount * ItemHas->ItemPrice)) return false;

	// 재료 소모
	for (const FCraftNeedItem& NeedItem : NeedItems)
	{
		UItemBase* LoadedItem = AssetManager.LoadItem(NeedItem.NeedItem);
	
		InventorySS->AddItem(LoadedItem, -NeedItem.NeedQuantity);
	}

	ItemCount -= CraftCount;

	// 인벤토리에 제작한 아이템을 추가
	InventorySS->AddItem(ItemHas, CraftCount);

	UpdateItemSlot(ItemHas, ItemCount);

	// 상점 제작 데이터 Map에 업데이트
	GetGameInstance()->GetSubsystem<UGISS_InteractableNpcData>()->ShopCraftItemQuantityMap.Add(ItemHas->GetPrimaryAssetId(), ItemCount);

	return true;
}
*/