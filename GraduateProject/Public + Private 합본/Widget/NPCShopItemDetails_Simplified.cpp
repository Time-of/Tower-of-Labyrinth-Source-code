
#include "Widget/NPCShopItemDetails_Simplified.h"

#include "GameAssetManager.h"
#include "GameInstance/GameInstanceBase.h"
#include "Item/ItemBase.h"
#include "Item/Item_WeaponBase.h"

#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"



UNPCShopItemDetails_Simplified::UNPCShopItemDetails_Simplified(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	ItemNameText(nullptr),
	ItemTypeText(nullptr),
	AdditionalDescriptionText(nullptr),
	ItemDescriptionText(nullptr)
{

}



void UNPCShopItemDetails_Simplified::NativeConstruct()
{
	Super::NativeConstruct();

}



void UNPCShopItemDetails_Simplified::NativeDestruct()
{
	Super::NativeDestruct();

}



void UNPCShopItemDetails_Simplified::UpdateItem(const UItemBase* Item)
{
	if (!IsValid(Item)) return;

	ItemNameText->SetText(Item->ItemName);
	ItemTypeText->SetText(FText::FromString(Item->ItemType.ToString()));
	ItemDescriptionText->SetText(Item->ItemDescription);

	SetAdditionalDescriptionText(Item);
}



void UNPCShopItemDetails_Simplified::SetAdditionalDescriptionText(const UItemBase* Item)
{
	if (Item->ItemType == UGameAssetManager::ItemType_Weapon)
	{
		UGameInstanceBase* GIBase = GetGameInstance<UGameInstanceBase>();
		FWeaponStatDataTableRow* WeaponStatRow = GIBase->GetWeaponStat(Item->InternalName);

		if (WeaponStatRow == nullptr) return;

		AdditionalDescriptionText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

#define LOCTEXT_NAMESPACE "WeaponDescriptionNamespace"
		AdditionalDescriptionText->SetText(FText::Format(
			LOCTEXT("WeaponDescriptionText", "공격력: {0}"),
			FText::AsNumber(WeaponStatRow->AtkPower))
		);
#undef LOCTEXT_NAMESPACE
	}
	else if (Item->ItemType == UGameAssetManager::ItemType_Armor)
	{
		UGameInstanceBase* GIBase = GetGameInstance<UGameInstanceBase>();
		FArmorStatDataTableRow* ArmorStatRow = GIBase->GetArmorStat(Item->InternalName);

		if (ArmorStatRow == nullptr) return;

		AdditionalDescriptionText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

#define LOCTEXT_NAMESPACE "ArmorDescriptionNamespace"
		AdditionalDescriptionText->SetText(FText::Format(
			LOCTEXT("ArmorDescriptionText", "보호량: {0}"),
			FText::AsNumber(ArmorStatRow->GuardGuageToAcquire))
		);
#undef LOCTEXT_NAMESPACE
	}
	else
	{
		AdditionalDescriptionText->SetVisibility(ESlateVisibility::Hidden);
	
		AdditionalDescriptionText->SetText(FText::GetEmpty());
	}
}
