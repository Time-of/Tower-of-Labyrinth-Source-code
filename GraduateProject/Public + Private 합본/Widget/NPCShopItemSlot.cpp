
#include "Widget/NPCShopItemSlot.h"

#include "CharacterBase.h"
#include "GameAssetManager.h"
#include "GameInstance/GISS_InteractableNpcData.h"
#include "GameInstance/GISS_PlayerInventory.h"
#include "GameInstance/GISS_PlayerMoney.h"
#include "Item/ItemBase.h"
#include "Item/Item_CharacterSkillBase.h"
#include "NPC/Shop/ShopSlotInteractStrategy.h"
#include "Widget/NPCShopConfirmInteract.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"



UNPCShopItemSlot::UNPCShopItemSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	InteractType(EShopInteractType::NONE), ItemHas(nullptr), ItemCount(0),
	ItemButton(nullptr), ItemImage(nullptr), ItemCountText(nullptr), ItemCountOverlay(nullptr),
	InteractStrategy(nullptr)
{
	static ConstructorHelpers::FClassFinder<UNPCShopConfirmInteract> ConfirmWBPClass(TEXT("/Game/SungSoo_Workspace/Blueprints/Widget/NPC/WBP_Shop_ConfirmInteract"));

	if (ConfirmWBPClass.Succeeded())
	{
		ConfirmWidgetClass = ConfirmWBPClass.Class;
	}
}



void UNPCShopItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateItemSlot(nullptr, 0);

	ItemButton->OnClicked.AddDynamic(this, &UNPCShopItemSlot::ShowConfirmWidgetOnButtonClicked);
}



void UNPCShopItemSlot::NativeDestruct()
{
	Super::NativeDestruct();

	if (InteractStrategy != nullptr)
	{
		delete InteractStrategy;
		InteractStrategy = nullptr;
	}
}



void UNPCShopItemSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	MouseEnterDelegate.Broadcast(ItemHas, InteractType);
}



void UNPCShopItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	MouseLeaveDelegate.Broadcast(ItemHas);
}



void UNPCShopItemSlot::SetInteractStrategy(ShopSlotInteractStrategy* NewStrategy)
{
	if (NewStrategy != nullptr)
	{
		InteractStrategy = NewStrategy;
	}
	// NewStrategy가 nullptr이라면
	else if (InteractStrategy != nullptr)
	{
		delete InteractStrategy;
		InteractStrategy = nullptr;
	}
}



void UNPCShopItemSlot::UpdateItemSlot(UItemBase* Item, int32 NewItemCount)
{
	ItemHas = Item;
	ItemCount = NewItemCount;

	if (!IsValid(ItemHas))
	{
		SetVisibility(ESlateVisibility::Hidden);

		return;
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
	}

	SetItemImage();

	if (ItemCount > 1)
	{
		ItemCountOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
		ItemCountText->SetText(FText::AsNumber(ItemCount));
	}
	else
	{
		ItemCountOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractType != EShopInteractType::CRAFT)
	{
		ItemButton->SetIsEnabled(ItemCount > 0);
	}
	// 제작인 경우
	else
	{
		UGISS_InteractableNpcData* NpcDataSS = GetGameInstance()->GetSubsystem<UGISS_InteractableNpcData>();

		// 제작 가능 여부를 따진다. (유효한 아이템, 맵에 포함되어 있음, 맵의 Value가 true)
		bool bCanBeCrafted = IsValid(ItemHas) && NpcDataSS->ShopCraftItem_CanBeCraftedMap.Contains(ItemHas->GetPrimaryAssetId()) &&
			NpcDataSS->ShopCraftItem_CanBeCraftedMap[ItemHas->GetPrimaryAssetId()];

		ItemButton->SetIsEnabled(ItemCount > 0 && bCanBeCrafted);
		LockBorder->SetVisibility(bCanBeCrafted ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
}



void UNPCShopItemSlot::OnInventoryUpdatedFromAssetId(FPrimaryAssetId Item, int32 NewItemCount)
{
	UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();

	OnInventoryUpdated(AssetManager.LoadItem(Item), NewItemCount);
}



void UNPCShopItemSlot::OnInventoryUpdated(UItemBase* Item, int32 NewItemCount)
{
	if (ItemHas != Item) return;

	UpdateItemSlot(Item, NewItemCount);
}



void UNPCShopItemSlot::InteractItem(UNPCShopConfirmInteract* ConfirmWidget, int32 InteractItemCount)
{
	if (InteractStrategy != nullptr && InteractStrategy->TryInteract(this, ItemHas, ItemCount, InteractItemCount))
	{
		UpdateItemSlot(ItemHas, ItemCount);
		ConfirmWidget->RemoveFromParent();
	}
}



void UNPCShopItemSlot::ShowConfirmWidgetOnButtonClicked()
{
	if (UNPCShopConfirmInteract* ConfirmWidget = CreateWidget<UNPCShopConfirmInteract>(this, ConfirmWidgetClass))
	{
		ConfirmWidget->UpdateInfo(this);

		ConfirmWidget->OnConfirmDelegate.AddUniqueDynamic(this, &UNPCShopItemSlot::InteractItem);

		ConfirmWidget->AddToViewport(12);
	}
}



void UNPCShopItemSlot::SetItemImage()
{
	FSlateBrush NewBrush = ItemHas->ItemIcon;

	NewBrush.ImageSize = FVector2D(128.0f, 128.0f);

	ItemImage->SetBrush(NewBrush);
}
