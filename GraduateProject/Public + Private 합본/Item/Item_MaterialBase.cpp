
#include "Item/Item_MaterialBase.h"

#include "GameAssetManager.h"



UItem_MaterialBase::UItem_MaterialBase() : Super()
{
	ItemType = UGameAssetManager::ItemType_Material;
}
