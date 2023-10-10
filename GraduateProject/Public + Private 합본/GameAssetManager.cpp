
#include "GameAssetManager.h"

#include "Item/ItemBase.h"

#include "AbilitySystemGlobals.h"



const FPrimaryAssetType UGameAssetManager::ItemType_Weapon = TEXT("Weapon");
const FPrimaryAssetType UGameAssetManager::ItemType_Armor = TEXT("Armor");
const FPrimaryAssetType UGameAssetManager::ItemType_Potion = TEXT("Potion");
const FPrimaryAssetType UGameAssetManager::ItemType_Material = TEXT("Material");
const FPrimaryAssetType UGameAssetManager::ItemType_CharacterSkill = TEXT("CharacterSkill");



UGameAssetManager::UGameAssetManager() : Super()
{

}



UGameAssetManager& UGameAssetManager::GetInstance()
{
	UGameAssetManager* AssetManager = Cast<UGameAssetManager>(GEngine->AssetManager);

	if (!AssetManager->IsValid()) UE_LOG(LogTemp, Fatal, TEXT("오류! AssetManager가 잘못 설정됨!"));

	return *AssetManager;
}



UItemBase* UGameAssetManager::LoadItem(const FPrimaryAssetId& PrimaryAssetId)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	UItemBase* LoadedItem = Cast<UItemBase>(ItemPath.TryLoad());

	return LoadedItem;
}



void UGameAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
