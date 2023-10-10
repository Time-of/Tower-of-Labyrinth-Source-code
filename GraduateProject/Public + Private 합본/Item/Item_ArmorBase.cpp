
#include "Item/Item_ArmorBase.h"

#include "CharacterBase.h"
#include "GameAssetManager.h"

#include "Kismet/GameplayStatics.h"



UItem_ArmorBase::UItem_ArmorBase() : Super()
{
	ItemType = UGameAssetManager::ItemType_Armor;
}



bool UItem_ArmorBase::TryEquipItem()
{
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GWorld, 0)))
	{
		CharacterBase->EquipArmor(this);

		return true;
	}

	return false;
}
