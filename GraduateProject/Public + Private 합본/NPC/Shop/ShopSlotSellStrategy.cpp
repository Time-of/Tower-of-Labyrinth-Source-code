
#include "NPC/Shop/ShopSlotSellStrategy.h"

#include "CharacterBase.h"
#include "GameAssetManager.h"
#include "GameInstance/GISS_InteractableNpcData.h"
#include "GameInstance/GISS_PlayerInventory.h"
#include "GameInstance/GISS_PlayerMoney.h"
#include "Item/ItemBase.h"
#include "Item/Item_CharacterSkillBase.h"
#include "UObject/PrimaryAssetId.h"
#include "Widget/NPCShopItemSlot.h"



ShopSlotSellStrategy::ShopSlotSellStrategy()
{
}



bool ShopSlotSellStrategy::TryInteract(UNPCShopItemSlot* Context, UItemBase* Item, int& CurrentItemQuantity, int InteractQuantity)
{
	UGISS_PlayerInventory* InvSS = Context->GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();

	if (CurrentItemQuantity <= 0 || InteractQuantity > CurrentItemQuantity || !IsValid(Item) || InvSS->InventoryMap[Item].Quantity < InteractQuantity) return false;

	CurrentItemQuantity -= InteractQuantity;

	Context->GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>()->AddMoney(Item->ItemPrice * InteractQuantity * 0.2f);

	InvSS->AddItem(Item, -InteractQuantity);

	if (UGISS_InteractableNpcData* NpcDataSS = Context->GetGameInstance()->GetSubsystem<UGISS_InteractableNpcData>())
	{
		if (NpcDataSS->ShopBuyItemQuantityMap.Contains(Item->GetPrimaryAssetId()))
		{
			NpcDataSS->SetShopBuyItemMap(Item->GetPrimaryAssetId(), 1);
		}
	}

	return true;
}
