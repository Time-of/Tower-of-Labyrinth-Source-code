
#include "GameInstance/GameInstanceBase.h"

#include "GameAssetManager.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/DataTable/EnemyDropTableRow.h"
#include "GameInstance/GISS_PlayerInventory.h"
#include "Item/ItemBase.h"
#include "Item/ItemPickupBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"



UGameInstanceBase::UGameInstanceBase() : UGameInstance(),
	EnemyDropTable(nullptr),
	WeightedWeaponTable(nullptr),
	EnemyStatAttributeTable(nullptr),
	WeaponStatDataTable(nullptr),
	ArmorStatDataTable(nullptr),
	ItemPickupClass(nullptr),
	WeaponStatEffectBaseClass(nullptr),
	ArmorStatEffectBaseClass(nullptr),
	LoadingScreenWidget(nullptr),
	LoadingScreenInstance(nullptr)
{
	ItemTypeArray = { UGameAssetManager::ItemType_Weapon,
			UGameAssetManager::ItemType_Armor,
			UGameAssetManager::ItemType_Potion,
			UGameAssetManager::ItemType_Material,
			UGameAssetManager::ItemType_CharacterSkill };

	static ConstructorHelpers::FObjectFinder<UDataTable> DropTableAsset(TEXT("/Game/SungSoo_Workspace/DataTables/DropTable/EnemyDropTable"));
	if (DropTableAsset.Succeeded())
	{
		EnemyDropTable = DropTableAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> WeightedWeaponTableAsset(TEXT("/Game/SungSoo_Workspace/DataTables/LevelEvent/WeightedRandomWeaponTable"));
	if (DropTableAsset.Succeeded())
	{
		WeightedWeaponTable = WeightedWeaponTableAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> EnemyStatTableAsset(TEXT("/Game/SungSoo_Workspace/DataTables/Stats/EnemyStatAttributesTable"));
	if (EnemyStatTableAsset.Succeeded())
	{
		EnemyStatAttributeTable = EnemyStatTableAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponStatTableAsset(TEXT("/Game/SungSoo_Workspace/DataTables/Stats/WeaponStatDataTable"));
	if (WeaponStatTableAsset.Succeeded())
	{
		WeaponStatDataTable = WeaponStatTableAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ArmorStatTableAsset(TEXT("/Game/SungSoo_Workspace/DataTables/Stats/ArmorStatDataTable"));
	if (ArmorStatTableAsset.Succeeded())
	{
		ArmorStatDataTable = ArmorStatTableAsset.Object;
	}

	static ConstructorHelpers::FClassFinder<AItemPickupBase> ItemPickupBPClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Pickup/BP_ItemPickup");
	if (ItemPickupBPClass.Succeeded())
	{
		ItemPickupClass = ItemPickupBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UGameplayEffect> DefaultWeaponStatClass(TEXT("/Game/SungSoo_Workspace/Blueprints/Effects/Items/Weapons/GEBase_WeaponStat"));
	if (DefaultWeaponStatClass.Succeeded())
	{
		WeaponStatEffectBaseClass = DefaultWeaponStatClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UGameplayEffect> DefaultArmorStatClass(TEXT("/Game/SungSoo_Workspace/Blueprints/Effects/Items/Armors/GEBase_ArmorStat"));
	if (DefaultArmorStatClass.Succeeded())
	{
		ArmorStatEffectBaseClass = DefaultArmorStatClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> LoadingScreenWidgetClass(TEXT("/Game/SungSoo_Workspace/Blueprints/Widget/WBP_LoadingScreen"));
	if (LoadingScreenWidgetClass.Succeeded())
	{
		LoadingScreenWidget = LoadingScreenWidgetClass.Class;
	}
}



void UGameInstanceBase::Init()
{
	UGameInstance::Init();

	TArray<FPrimaryAssetId> OutAssetIdList;

	if (UAssetManager* AssetManager = UAssetManager::GetIfValid())
	{
		// 모든 아이템 타입에 대하여
		for (const FPrimaryAssetType& Type : ItemTypeArray)
		{
			// 해당 타입의 모든 PrimaryAssetId를 가져온다.
			UKismetSystemLibrary::GetPrimaryAssetIdList(Type, OutAssetIdList);

			InitializeItems(OutAssetIdList);
		}
	}
}



FEnemyDropTableRow* UGameInstanceBase::GetEnemyDropTableRow(const FName& InternalEnemyName) const
{
	if (FEnemyDropTableRow* DropTableRow = EnemyDropTable->FindRow<FEnemyDropTableRow>(InternalEnemyName, TEXT("")))
	{
		return DropTableRow;
	}

	return nullptr;
}



FPrimaryAssetId UGameInstanceBase::GetItemByTypeAndName(const FPrimaryAssetType& PrimaryAssetType, const FName& PrimaryAssetName)
{
	for (const UItemBase* Item : ItemsArray)
	{
		FPrimaryAssetId ItemPrimaryAssetId = Item->GetPrimaryAssetId();

		if (ItemPrimaryAssetId.PrimaryAssetType == PrimaryAssetType && ItemPrimaryAssetId.PrimaryAssetName == PrimaryAssetName)
		{
			return ItemPrimaryAssetId;
		}
	}

	UE_LOG(LogTemp, Fatal, TEXT("%s 아이템 찾지 못함!!"), *PrimaryAssetName.ToString());

	return FPrimaryAssetId();
}



TSubclassOf<AItemPickupBase> UGameInstanceBase::GetItemPickupClass() const
{
	return ItemPickupClass;
}



UItemBase* UGameInstanceBase::GetRandomItemFromWeightedWeaponTable() const
{
	return GetRandomItemFromWeightedTable(WeightedWeaponTable);
}



UItemBase* UGameInstanceBase::GetWeaponFromWeightedWeaponTableInWeightRange(float MinWeight, float MaxWeight)
{
	TArray<FWeightedItemTableRow*> OutRows;
	WeightedWeaponTable->GetAllRows<FWeightedItemTableRow>(TEXT(""), OutRows);

	TArray<FWeightedItemTableRow*> FoundRows;

	for (auto& Row : OutRows)
	{
		if (MinWeight <= Row->Weight && Row->Weight <= MaxWeight)
		{
			FoundRows.Add(Row);
		}
	}

	return _GetRandomItemInWeightedRows(FoundRows);
}



UItemBase* UGameInstanceBase::GetRandomItemFromWeightedTable(UDataTable* WeightedTable) const
{
	TArray<FWeightedItemTableRow*> OutRows;
	WeightedTable->GetAllRows<FWeightedItemTableRow>(TEXT(""), OutRows);

	return _GetRandomItemInWeightedRows(OutRows);
}



UItemBase* UGameInstanceBase::_GetRandomItemInWeightedRows(const TArray<FWeightedItemTableRow*>& RowArray) const
{
	float TotalWeight = 0.0f;

	for (const auto& Row : RowArray)
	{
		TotalWeight += Row->Weight;
	}

	if (TotalWeight <= 0.0f) return nullptr;

	float Pivot = UKismetMathLibrary::RandomFloatInRange(0.0f, TotalWeight);

	for (const auto& Row : RowArray)
	{
		if (Pivot < Row->Weight)
		{
			return Row->Item;
		}
		else
		{
			Pivot -= Row->Weight;
		}
	}

	return nullptr;
}



float UGameInstanceBase::GetWeaponWeightInWeightedTable(const FName& ItemInternalName)
{
	TArray<FWeightedItemTableRow*> OutRows;
	WeightedWeaponTable->GetAllRows<FWeightedItemTableRow>(TEXT(""), OutRows);

	for (auto& Row : OutRows)
	{
		if (Row->Item->InternalName == ItemInternalName)
		{
			return Row->Weight;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("오류!!! GetWeaponWeightInWeightedTable(): 아이템 %s 읽어올 수 없음"), *ItemInternalName.ToString());

	return 100.0f;
}



FStatAttributeInfo UGameInstanceBase::GetStatByLevel(const FName& InternalName, int32 Level) const
{
	if (FLevelBasedStatAttributeTableRow* FoundRow = EnemyStatAttributeTable->FindRow<FLevelBasedStatAttributeTableRow>(InternalName, TEXT("")))
	{
		return FoundRow->GetStatByLevel(Level);
	}

	UE_LOG(LogTemp, Warning, TEXT("오류!!!: GameInstanceBase의 GetStatByLevel(): 테이블에서 %s 행 읽어올 수 없음"), *InternalName.ToString());

	FStatAttributeInfo OnNotFound;
	OnNotFound.MaxHealth = 1.0f;

	return OnNotFound;
}



FGameplayEffectSpecHandle UGameInstanceBase::MakeWeaponStatEffect(const FName& InternalName, UAbilitySystemComponent* ASC)
{
	FGameplayEffectSpecHandle WeaponStatEffectSpecHandle = ASC->MakeOutgoingSpec(WeaponStatEffectBaseClass,
		0.0f, ASC->MakeEffectContext());

	float AtkPower = 0.0f;
	float AtkSpeedRate = 1.0f;
	float AtkRangeRate = 1.0f;

	if (FWeaponStatDataTableRow* WeaponStatRow = GetWeaponStat(InternalName))
	{
		AtkPower = WeaponStatRow->AtkPower;
		AtkSpeedRate = WeaponStatRow->AtkSpeedRate;
		AtkRangeRate = WeaponStatRow->AtkRangeRate;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstanceBase의 MakeWeaponStatEffect: 무기 '%s' 능력치 행 찾을 수 없음!!!!!"), *InternalName.ToString());
	}

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		WeaponStatEffectSpecHandle,
		FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.AtkPower")),
		AtkPower
	);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		WeaponStatEffectSpecHandle,
		FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.AtkSpeedRate")),
		AtkSpeedRate
	);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		WeaponStatEffectSpecHandle,
		FGameplayTag::RequestGameplayTag(TEXT("Data.Weapon.AtkRangeRate")),
		AtkRangeRate
	);

	return WeaponStatEffectSpecHandle;
}



FWeaponStatDataTableRow* UGameInstanceBase::GetWeaponStat(const FName& InternalName) const
{
	if (FWeaponStatDataTableRow* WeaponStatTableRow = WeaponStatDataTable->FindRow<FWeaponStatDataTableRow>(InternalName, TEXT("")))
	{
		return WeaponStatTableRow;
	}

	return nullptr;
}



FGameplayEffectSpecHandle UGameInstanceBase::MakeArmorStatEffect(const FName& InternalName, UAbilitySystemComponent* ASC)
{
	FGameplayEffectSpecHandle ArmorStatEffectSpecHandle = ASC->MakeOutgoingSpec(ArmorStatEffectBaseClass,
		0.0f, ASC->MakeEffectContext());

	float GuardProvide = 0.0f;

	if (FArmorStatDataTableRow* ArmorStatRow = GetArmorStat(InternalName))
	{
		GuardProvide = ArmorStatRow->GuardGuageToAcquire;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstanceBase의 MakeArmorStatEffect: 방어구 '%s' 능력치 행 찾을 수 없음!!!!!"), *InternalName.ToString());
	}

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		ArmorStatEffectSpecHandle,
		FGameplayTag::RequestGameplayTag(TEXT("Data.Armor.GuardGuageToAcquire")),
		GuardProvide
	);

	return ArmorStatEffectSpecHandle;
}



FArmorStatDataTableRow* UGameInstanceBase::GetArmorStat(const FName& InternalName) const
{
	if (FArmorStatDataTableRow* ArmorStatTableRow = ArmorStatDataTable->FindRow<FArmorStatDataTableRow>(InternalName, TEXT("")))
	{
		return ArmorStatTableRow;
	}

	return nullptr;
}



void UGameInstanceBase::ShowLoadingScreen()
{
	if (!IsValid(LoadingScreenWidget)) return;

	if (!IsValid(LoadingScreenInstance))
	{
		LoadingScreenInstance =
			CreateWidget(this, LoadingScreenWidget);
	}
	
	LoadingScreenInstance->AddToViewport(100);
}



void UGameInstanceBase::HideLoadingScreen()
{
	if (IsValid(LoadingScreenInstance))
	{
		LoadingScreenInstance->RemoveFromParent();
		LoadingScreenInstance = nullptr;
		return;
	}
}
