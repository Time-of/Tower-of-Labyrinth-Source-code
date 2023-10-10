
#include "GameInstance/GISS_InteractableNpcData.h"

#include "GameAssetManager.h"
#include "GameInstance/GameInstanceBase.h"
#include "Item/ItemBase.h"
#include "NPC/DataTable/ShopCraftNeedItemsTableRow.h"
#include "NPC/DataTable/ShopDataTableRow.h"
#include "SaveGame/SaveGame_Shop.h"

#include "Kismet/GameplayStatics.h"

#include <GraduateProject/GraduateProject.h>



UGISS_InteractableNpcData::UGISS_InteractableNpcData() : Super()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ShopDataTableAsset(TEXT("/Game/SungSoo_Workspace/DataTables/InteractableNpc/NpcShopDataTable"));

	if (ShopDataTableAsset.Succeeded())
	{
		ShopDataTable = ShopDataTableAsset.Object;
	}


	static ConstructorHelpers::FObjectFinder<UDataTable> ShopCraftNeedItemsDataTableAsset(TEXT("/Game/SungSoo_Workspace/DataTables/InteractableNpc/NpcShopCraftNeedItemsDataTable"));

	if (ShopCraftNeedItemsDataTableAsset.Succeeded())
	{
		ShopCraftNeedItemsDataTable = ShopCraftNeedItemsDataTableAsset.Object;
	}
}



void UGISS_InteractableNpcData::Initialize(FSubsystemCollectionBase& Collection)
{
	InitializeShopData();
}



TArray<FCraftNeedItem> UGISS_InteractableNpcData::GetCraftNeedItems(const FName& InternalItemName) const
{
	if (FShopCraftNeedItemsDataTableRow* FoundRow = ShopCraftNeedItemsDataTable->FindRow<FShopCraftNeedItemsDataTableRow>(InternalItemName, TEXT("")))
	{
		return FoundRow->NeedItems;
	}

	UE_LOG(LogTemp, Warning, TEXT("GetCraftNeedItems에서 %s 행 찾지 못함"), *InternalItemName.ToString());

	return TArray<FCraftNeedItem>();
}



void UGISS_InteractableNpcData::SaveNpcData(int32 SlotIndex)
{
	USaveGame_Shop* CreatedShopSaveGame = Cast<USaveGame_Shop>(UGameplayStatics::CreateSaveGameObject(USaveGame_Shop::StaticClass()));

	CreatedShopSaveGame->ShopBuyItemQuantityMap = ShopBuyItemQuantityMap;
	CreatedShopSaveGame->ShopCraftItemQuantityMap = ShopCraftItemQuantityMap;
	CreatedShopSaveGame->ShopCraftItem_CanBeCraftedMap = ShopCraftItem_CanBeCraftedMap;

	UGameplayStatics::SaveGameToSlot(CreatedShopSaveGame, TEXT("ShopNpcSaveGame"), SlotIndex);

	UE_LOG(LoadPlayerDataLog, Warning, TEXT("저장: NPC 데이터 완료"));
}



void UGISS_InteractableNpcData::LoadNpcData(int32 SlotIndex)
{
	ShopBuyItemQuantityMap.Reset();
	ShopCraftItemQuantityMap.Reset();
	ShopCraftItem_CanBeCraftedMap.Reset();

	// 상점 데이터 저장 세이브 게임이 있다면 불러온다.
	if (UGameplayStatics::DoesSaveGameExist(TEXT("ShopNpcSaveGame"), SlotIndex))
	{
		USaveGame_Shop* SavedShopData = Cast<USaveGame_Shop>(UGameplayStatics::LoadGameFromSlot(TEXT("ShopNpcSaveGame"), SlotIndex));

		ShopBuyItemQuantityMap = SavedShopData->ShopBuyItemQuantityMap;
		ShopCraftItemQuantityMap = SavedShopData->ShopCraftItemQuantityMap;
		ShopCraftItem_CanBeCraftedMap = SavedShopData->ShopCraftItem_CanBeCraftedMap;

		UE_LOG(LoadPlayerDataLog, Warning, TEXT("불러오기: NPC 데이터 완료"));
	}
	// 상점 데이터 저장 세이브 게임이 없다면 데이터 테이블을 읽어 새로 만든다.
	else
	{
		InitializeShopData();

		UE_LOG(LoadPlayerDataLog, Warning, TEXT("불러오기 실패: 저장된 NPC 데이터 존재하지 않음"));
	}
}



void UGISS_InteractableNpcData::SetShopBuyItemMap(const FPrimaryAssetId& Item, int32 NewQuantity)
{
	ShopBuyItemQuantityMap.Add(Item, NewQuantity);
	OnShopBuyItemQuantityChanged.Broadcast(Item, NewQuantity);
}



void UGISS_InteractableNpcData::InitializeShopData()
{
	ShopBuyItemQuantityMap.Reset();
	ShopCraftItemQuantityMap.Reset();
	ShopCraftItem_CanBeCraftedMap.Reset();

	TArray<FShopDataTableRow*> ShopTableRowArray;
	ShopDataTable->GetAllRows<FShopDataTableRow>(TEXT(""), ShopTableRowArray);

	for (const FShopDataTableRow* Row : ShopTableRowArray)
	{
		switch (Row->InteractType)
		{
		case EShopInteractType::BUY:
			ShopBuyItemQuantityMap.Add(Row->Item, Row->ItemQuantity);
			break;

		case EShopInteractType::CRAFT:
			ShopCraftItemQuantityMap.Add(Row->Item, Row->ItemQuantity);
			break;

			// 판매는 목록에 끼워넣지 않음
		default:
		case EShopInteractType::NONE:
		case EShopInteractType::SELL:
			break;
		}
	}

	UGameInstanceBase* GIBase = Cast<UGameInstanceBase>(GetGameInstance());

	if (IsValid(GIBase))
	{
		ShopCraftItem_CanBeCraftedMap = GIBase->Default_Shop_CraftItem_CanBeCrafted_Map;
	}
}
