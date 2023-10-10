
#include "Item/Item_PotionBase.h"

#include "GameAssetManager.h"



UItem_PotionBase::UItem_PotionBase() : Super(),
	EffectClassToApplyToTarget(nullptr),
	EffectLevel(1)
{
	ItemType = UGameAssetManager::ItemType_Potion;
}
