
#include "NPC/Shop/ShopSlotCraftStrategy.h"

#include "CharacterBase.h"
#include "GameAssetManager.h"
#include "GameInstance/GISS_InteractableNpcData.h"
#include "GameInstance/GISS_PlayerInventory.h"
#include "GameInstance/GISS_PlayerMoney.h"
#include "Item/ItemBase.h"
#include "Item/Item_CharacterSkillBase.h"
#include "UObject/PrimaryAssetId.h"
#include "Widget/NPCShopItemSlot.h"



ShopSlotCraftStrategy::ShopSlotCraftStrategy() : NeedItems()
{
}



ShopSlotCraftStrategy::ShopSlotCraftStrategy(const TArray<FCraftNeedItem>& NewNeedItems) : NeedItems(NewNeedItems)
{
}



bool ShopSlotCraftStrategy::TryInteract(UNPCShopItemSlot* Context, UItemBase* Item, int& CurrentItemQuantity, int InteractQuantity)
{
	if (CurrentItemQuantity <= 0 || InteractQuantity > CurrentItemQuantity || !IsValid(Item)) return false;

	UGISS_InteractableNpcData* NpcDataSS = Context->GetGameInstance()->GetSubsystem<UGISS_InteractableNpcData>();

	// 만약 제작 불가능하다고 판단되면 (제작가능 맵에 없거나, 있는데 Value가 false인 경우)
	if (!NpcDataSS->ShopCraftItem_CanBeCraftedMap.Contains(Item->GetPrimaryAssetId()) ||
		!NpcDataSS->ShopCraftItem_CanBeCraftedMap[Item->GetPrimaryAssetId()]) return false;

	UGISS_PlayerInventory* InventorySS = Context->GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();
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
	if (!Context->GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>()->TryUseMoney(InteractQuantity * Item->ItemPrice)) return false;

	// 재료 소모
	for (const FCraftNeedItem& NeedItem : NeedItems)
	{
		UItemBase* LoadedItem = AssetManager.LoadItem(NeedItem.NeedItem);

		InventorySS->AddItem(LoadedItem, -NeedItem.NeedQuantity);
	}

	CurrentItemQuantity -= InteractQuantity;

	// 인벤토리에 제작한 아이템을 추가
	InventorySS->AddItem(Item, InteractQuantity);

	// 상점 제작 데이터 Map에 업데이트
	NpcDataSS->ShopCraftItemQuantityMap.Add(Item->GetPrimaryAssetId(), CurrentItemQuantity);

	return true;
}
