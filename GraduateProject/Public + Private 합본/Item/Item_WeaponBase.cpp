
#include "Item/Item_WeaponBase.h"

#include "CharacterBase.h"
#include "GameAssetManager.h"

#include "Kismet/GameplayStatics.h"



UItem_WeaponBase::UItem_WeaponBase() : Super(),
	AdditionalEffectOnAttackClass(nullptr),
	WeaponMesh(nullptr),
	GrantedSkillOnEquip(nullptr),
	bUseSkillImmediately(false)
{
	ItemType = UGameAssetManager::ItemType_Weapon;
}



bool UItem_WeaponBase::TryEquipItem()
{
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GWorld, 0)))
	{
		CharacterBase->EquipWeapon(this);

		return true;
	}

	return false;
}

