
#include "NPC/Shop/ShopSlotBuyStrategy.h"

#include "CharacterBase.h"
#include "GameAssetManager.h"
#include "GameInstance/GISS_InteractableNpcData.h"
#include "GameInstance/GISS_PlayerInventory.h"
#include "GameInstance/GISS_PlayerMoney.h"
#include "Item/ItemBase.h"
#include "Item/Item_CharacterSkillBase.h"
#include "UObject/PrimaryAssetId.h"
#include "Widget/NPCShopItemSlot.h"



ShopSlotBuyStrategy::ShopSlotBuyStrategy()
{
}



bool ShopSlotBuyStrategy::TryInteract(UNPCShopItemSlot* Context, UItemBase* Item, int& CurrentItemQuantity, int InteractQuantity)
{
	if (CurrentItemQuantity <= 0 || InteractQuantity > CurrentItemQuantity || !IsValid(Item)) return false;

	UGameInstance* GI = Context->GetGameInstance();

	if (!GI->GetSubsystem<UGISS_PlayerMoney>()->TryUseMoney(InteractQuantity * Item->ItemPrice)) return false;

	CurrentItemQuantity -= InteractQuantity;

	// 인벤토리에 구매한 만큼의 아이템을 추가
	GI->GetSubsystem<UGISS_PlayerInventory>()->AddItem(Item, InteractQuantity);

	UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();
	ACharacterBase* PlayerCharacterRef = Context->GetWorld()->GetFirstPlayerController()->GetPawn<ACharacterBase>();

	// 스킬이라면, 스킬을 바로 적용
	// 23.04.24 일시적으로 막아놨음!!!
	if (Item->ItemType == AssetManager.ItemType_CharacterSkill && IsValid(PlayerCharacterRef))
	{
		//PlayerCharacterRef->EquipSkillItem(Cast<UItem_CharacterSkillBase>(Item));
	}

	// 상점 데이터 Map에 업데이트
	GI->GetSubsystem<UGISS_InteractableNpcData>()->SetShopBuyItemMap(Item->GetPrimaryAssetId(), CurrentItemQuantity);

	return true;
}
