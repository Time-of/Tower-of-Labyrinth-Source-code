
#include "Widget/NPCShopWidget.h"

#include "CharacterBase.h"
#include "GameAssetManager.h"
#include "GameInstance/GISS_InteractableNpcData.h"
#include "GameInstance/GISS_PlayerMoney.h"
#include "GameInstance/GISS_PlayerInventory.h"
#include "Item/ItemBase.h"
#include "Item/Item_ArmorBase.h"
#include "Item/Item_CharacterSkillBase.h"
#include "Item/Item_WeaponBase.h"
#include "NPC/DataTable/ShopCraftNeedItemsTableRow.h"
#include "NPC/Shop/ShopSlotBuyStrategy.h"
#include "NPC/Shop/ShopSlotCraftStrategy.h"
#include "NPC/Shop/ShopSlotSellStrategy.h"
#include "Widget/NPCShopConfirmInteract.h"
#include "Widget/NPCShopItemDetails.h"
#include "Widget/NPCShopItemSlot.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"



UNPCShopWidget::UNPCShopWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	ItemSlotRow(3), ItemSlotColumn(5),
	ItemDetailsWidget(nullptr),
	MoneyText(nullptr),
	UniformGrid_Buy(nullptr),
	UniformGrid_Sell(nullptr),
	UniformGrid_Craft(nullptr),
	ItemSlotWidgetClass(nullptr),
	DisplayedType(EShopItemDisplayedType::WEAPONS_ONLY)
{
	static ConstructorHelpers::FClassFinder<UNPCShopItemSlot> DefaultItemSlotClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Widget/NPC/WBP_Shop_ItemSlot");

	if (DefaultItemSlotClass.Succeeded())
	{
		ItemSlotWidgetClass = DefaultItemSlotClass.Class;
	}
}



void UNPCShopWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetupItemSlots(UniformGrid_Buy, ItemSlotWidgetClass, EShopInteractType::BUY);
	SetupItemSlots(UniformGrid_Sell, ItemSlotWidgetClass, EShopInteractType::SELL);
	SetupItemSlots(UniformGrid_Craft, ItemSlotWidgetClass, EShopInteractType::CRAFT);
}



void UNPCShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HideItemDetailsWidget(nullptr);


	UGISS_PlayerMoney* MoneySS = GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>();

	UpdateMoneyText(MoneySS->GetMoney());
	MoneySS->OnMoneyChanged.AddUniqueDynamic(this, &UNPCShopWidget::UpdateMoneyText);


	UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();

	InvSS->OnInventoryChangedDelegate.AddUniqueDynamic(this, &UNPCShopWidget::OnInventoryUpdated);


	SetupShopItemsData();
	LoadSellItemFromInventory();
}



void UNPCShopWidget::NativeDestruct()
{
	Super::NativeDestruct();

	RemoveItemSlots(UniformGrid_Buy);
	RemoveItemSlots(UniformGrid_Sell);
	RemoveItemSlots(UniformGrid_Craft);


	UGISS_PlayerMoney* MoneySS = GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>();

	MoneySS->OnMoneyChanged.RemoveDynamic(this, &UNPCShopWidget::UpdateMoneyText);


	UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();

	InvSS->OnInventoryChangedDelegate.RemoveDynamic(this, &UNPCShopWidget::OnInventoryUpdated);
}



void UNPCShopWidget::SetupItemSlots(UUniformGridPanel* UniformGrid, TSubclassOf<UNPCShopItemSlot> ItemSlotClass, EShopInteractType InteractType)
{
	if (!IsValid(ItemSlotClass)) return;

	for (int i = 0; i < ItemSlotRow; i++)
	{
		for (int k = 0; k < ItemSlotColumn; k++)
		{
			UNPCShopItemSlot* CreatedSlotWidget = CreateWidget<UNPCShopItemSlot>(GetWorld(), ItemSlotClass);

			UniformGrid->AddChildToUniformGrid(CreatedSlotWidget, i, k);

			CreatedSlotWidget->MouseEnterDelegate.AddUniqueDynamic(this, &UNPCShopWidget::ShowItemDetailsWidget);
			CreatedSlotWidget->MouseLeaveDelegate.AddUniqueDynamic(this, &UNPCShopWidget::HideItemDetailsWidget);

			CreatedSlotWidget->InteractType = InteractType;
		}
	}
}



void UNPCShopWidget::RemoveItemSlots(UUniformGridPanel* UniformGrid)
{
	for (UWidget* Child : UniformGrid->GetAllChildren())
	{
		Child->RemoveFromParent();
	}
}



void UNPCShopWidget::SetupShopItemsData()
{
	UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();

	if (UGISS_InteractableNpcData* NpcDataSS = GetGameInstance()->GetSubsystem<UGISS_InteractableNpcData>())
	{
		int32 BuyIndex = 0;
		int32 CraftIndex = 0;

		// 구매 창 데이터 읽어서 순서대로 설정하기
		for (const TTuple<FPrimaryAssetId, int32>& BuyData : NpcDataSS->ShopBuyItemQuantityMap)
		{
			if (UItemBase* LoadedItem = AssetManager.LoadItem(BuyData.Key))
			{

				// 디스플레이 타입 Enum에 따라 무기만, 또는 방어구만 표시하도록 설정
				switch (DisplayedType)
				{
				case EShopItemDisplayedType::WEAPONS_ONLY:
				{
					if (LoadedItem->ItemType != UGameAssetManager::ItemType_Weapon) continue;
				}
				break;


				case EShopItemDisplayedType::ARMORS_ONLY:
				{
					if (LoadedItem->ItemType != UGameAssetManager::ItemType_Armor) continue;
				}
				break;


				default:
					break;
				}


				UNPCShopItemSlot* SlotWidget = SetItemSlotByIndex(UniformGrid_Buy, LoadedItem, BuyData.Value, BuyIndex++);

				// 구매 스트래티지 설정
				SlotWidget->SetInteractStrategy(new ShopSlotBuyStrategy());

				NpcDataSS->OnShopBuyItemQuantityChanged.AddUniqueDynamic(SlotWidget, &UNPCShopItemSlot::OnInventoryUpdatedFromAssetId);
			}
		}

		// 제작 창 데이터 읽어서 순서대로 설정하기
		for (const TTuple<FPrimaryAssetId, int32>& CraftData : NpcDataSS->ShopCraftItemQuantityMap)
		{
			if (UItemBase* LoadedItem = AssetManager.LoadItem(CraftData.Key))
			{

				// 디스플레이 타입 Enum에 따라 무기만, 또는 방어구만 표시하도록 설정
				switch (DisplayedType)
				{
				case EShopItemDisplayedType::WEAPONS_ONLY:
				{
					if (LoadedItem->ItemType != UGameAssetManager::ItemType_Weapon) continue;
				}
				break;


				case EShopItemDisplayedType::ARMORS_ONLY:
				{
					if (LoadedItem->ItemType != UGameAssetManager::ItemType_Armor) continue;
				}
				break;


				default:
					break;
				}

				UNPCShopItemSlot* CraftSlotWidget = SetItemSlotByIndex(UniformGrid_Craft, LoadedItem, CraftData.Value, CraftIndex++);

				// 제작 스트래티지 설정
				TArray<FCraftNeedItem> NeedItems = NpcDataSS->GetCraftNeedItems(LoadedItem->InternalName);

				if (NeedItems.IsValidIndex(0) && IsValid(CraftSlotWidget))
				{
					CraftSlotWidget->SetInteractStrategy(new ShopSlotCraftStrategy(NeedItems));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("제작 슬롯 %s 유효하지 않음!!!"), *CraftData.Key.PrimaryAssetName.ToString());
				}
			}
		}
	}
}



void UNPCShopWidget::LoadSellItemFromInventory()
{
	if (UGISS_PlayerInventory* InvRef = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>())
	{
		UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();

		int32 SellIndex = 0;

		UItem_WeaponBase* EquippedWeapon = nullptr;
		UItem_ArmorBase* EquippedArmor = nullptr;

		if (ACharacterBase* PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn<ACharacterBase>())
		{
			EquippedWeapon = PlayerCharacterRef->GetEquippedWeapon();
			EquippedArmor = PlayerCharacterRef->GetEquippedArmor();
		}

		// 인벤토리 맵을 읽어 슬롯에 설정
		for (const TPair<UItemBase*, FItemSlot>& ItemInMap : InvRef->InventoryMap)
		{
			int32 ItemQuantity = ItemInMap.Value.Quantity;

			// 무기인 경우
			if (ItemInMap.Key->ItemType == AssetManager.ItemType_Weapon)
			{
				// 장착중인 아이템은 띄우지 않도록 변경 (23.03.06)
				if (IsValid(EquippedWeapon) && ItemInMap.Key == EquippedWeapon)
				{
					//ItemQuantity--;
					continue;
				}
			}
			// 방어구인 경우
			else if (ItemInMap.Key->ItemType == AssetManager.ItemType_Armor)
			{
				if (IsValid(EquippedArmor) && ItemInMap.Key == EquippedArmor)
				{
					//ItemQuantity--;
					continue;
				}
			}
			// 스킬인 경우
			else if (ItemInMap.Key->ItemType == AssetManager.ItemType_CharacterSkill)
			{
				continue;
			}

			if (UNPCShopItemSlot* SlotWidget = SetItemSlotByIndex(UniformGrid_Sell, ItemInMap.Key, ItemQuantity, SellIndex++))
			{
				// 판매 스트래티지 설정
				SlotWidget->SetInteractStrategy(new ShopSlotSellStrategy());
			}

			//InvRef->OnInventoryChangedDelegate.AddUniqueDynamic(SlotWidget, &UNPCShopItemSlot::OnInventoryUpdated);
		}
	}
}



void UNPCShopWidget::ForceLoadSellItem()
{
	RemoveItemSlots(UniformGrid_Sell);
	SetupItemSlots(UniformGrid_Sell, ItemSlotWidgetClass, EShopInteractType::SELL);
	LoadSellItemFromInventory();
}



void UNPCShopWidget::ShowItemDetailsWidget(UItemBase* Item, EShopInteractType InteractType)
{
	if (!IsValid(Item)) return;

	if (InteractType != EShopInteractType::CRAFT)
	{
		ItemDetailsWidget->UpdateItem(Item);

		// 판매라면, 가격을 20%로 설정
		if (InteractType == EShopInteractType::SELL)
		{
			ItemDetailsWidget->UpdateMoneyText(Item->ItemPrice * 0.2f);
		}
	}
	else
	{
		UGISS_InteractableNpcData* NpcDataSS = GetGameInstance()->GetSubsystem<UGISS_InteractableNpcData>();

		ItemDetailsWidget->UpdateItem(Item, NpcDataSS->GetCraftNeedItems(Item->InternalName));

		bool bCanBeCrafted = IsValid(Item) && NpcDataSS->ShopCraftItem_CanBeCraftedMap.Contains(Item->GetPrimaryAssetId()) &&
			NpcDataSS->ShopCraftItem_CanBeCraftedMap[Item->GetPrimaryAssetId()];

		// 제작 불가능한 아이템이면 언노운 아이템(미확인 아이템) 처리 (텍스트를 모두 ???, 알 수 없음 정도로 변경)
		if (!bCanBeCrafted) ItemDetailsWidget->UpdateTextUnknownItem();
	}

	ItemDetailsWidget->SetInteractTextByType(InteractType);

	ItemDetailsWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}



void UNPCShopWidget::HideItemDetailsWidget(UItemBase* Item)
{
	ItemDetailsWidget->SetVisibility(ESlateVisibility::Collapsed);
}



void UNPCShopWidget::UpdateMoneyText(float NewAmount)
{
#define LOCTEXT_NAMESPACE "MoneyTextNameSpace"
	MoneyText->SetText(FText::Format(LOCTEXT("MoneyText", "GOLD: {0}"), FText::AsNumber(NewAmount)));
#undef LOCTEXT_NAMESPACE
}



void UNPCShopWidget::OnInventoryUpdated(UItemBase* Item, int32 NewItemCount, int32 DeltaQuantity)
{
	for (const auto& WidgetSlot : UniformGrid_Sell->GetAllChildren())
	{
		if (UNPCShopItemSlot* SellSlot = Cast<UNPCShopItemSlot>(WidgetSlot))
		{
			if (SellSlot->ItemHas == Item)
			{
				SellSlot->UpdateItemSlot(Item, NewItemCount);
				return;
			}
		}
	}

	// 찾지 못한 경우
	ForceLoadSellItem();
}



UNPCShopItemSlot* UNPCShopWidget::SetItemSlotByIndex(UUniformGridPanel* UniformGrid, UItemBase* ItemToSet, int32 NewItemCount, int32 Index)
{
	if (UWidget* ChildWidget = UniformGrid->GetChildAt(Index))
	{
		if (UNPCShopItemSlot* SlotWidget = Cast<UNPCShopItemSlot>(ChildWidget))
		{
			SlotWidget->UpdateItemSlot(ItemToSet, NewItemCount);

			return SlotWidget;
		}
	}

	return nullptr;
}
