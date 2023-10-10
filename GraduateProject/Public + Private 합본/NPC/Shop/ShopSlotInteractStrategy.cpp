
#include "NPC/Shop/ShopSlotInteractStrategy.h"



ShopSlotInteractStrategy::ShopSlotInteractStrategy()
{
}



ShopSlotInteractStrategy::~ShopSlotInteractStrategy()
{
	//UE_LOG(LogTemp, Log, TEXT("상점 스트래티지 %s 소멸됨"), *FString(typeid(this).name()));
}



bool ShopSlotInteractStrategy::TryInteract(class UNPCShopItemSlot* Context, UItemBase* Item, int& CurrentItemQuantity, int InteractQuantity)
{
	// null 객체와 같은 역할, 재정의 필요
	return false;
}
