
#include "Item/Item_CharacterSkillBase.h"

#include "GameAssetManager.h"
#include "Player/PlayerCharacterBase.h"

#include "Kismet/GameplayStatics.h"



UItem_CharacterSkillBase::UItem_CharacterSkillBase() : Super(),
	GrantedAbilityLevel(1)
{
	ItemType = UGameAssetManager::ItemType_CharacterSkill;
}



bool UItem_CharacterSkillBase::TryEquipItem()
{
	EquipSkillItem(EEnemyAbilityID::AbilityRM);

	return true;
}



void UItem_CharacterSkillBase::EquipSkillItem(EEnemyAbilityID SkillID)
{
	if (APlayerCharacterBase* PC = Cast<APlayerCharacterBase>(UGameplayStatics::GetPlayerCharacter(GWorld, 0)))
	{
		PC->EquipSkillItem(this, SkillID);
	}
}
