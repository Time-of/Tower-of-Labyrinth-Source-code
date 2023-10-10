
#include "Widget/NPCShopConfirmInteract.h"

#include "GameAssetManager.h"
#include "GameInstance/GameInstanceBase.h"
#include "GameInstance/GISS_InteractableNpcData.h"
#include "GameInstance/GISS_PlayerInventory.h"
#include "GameInstance/GISS_PlayerMoney.h"
#include "Item/ItemBase.h"
#include "NPC/DataTable/ShopCraftNeedItemsTableRow.h"
#include "NPC/Shop/ShopSlotCraftStrategy.h"
#include "Widget/NPCShopItemDetails_Simplified.h"
#include "Widget/NPCShopItemSlot.h"
#include "Widget/NPCShopNeedItemInfo.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"



UNPCShopConfirmInteract::UNPCShopConfirmInteract(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	Button_InteractConfirm(nullptr), Button_CancelConfirm(nullptr),
	ItemToInteract(nullptr), InteractType(EShopInteractType::NONE), Image_ItemIcon(nullptr),
	NeedInfo_Money(nullptr), VerticalBox_NeedItems(nullptr),
	ItemQuantityEditableText(nullptr), Button_AddQuantity(nullptr), Button_SubQuantity(nullptr),
	InteractConfirmButtonText(nullptr), ItemDetailsWidget(nullptr),
	EnteredQuantity(0), MaxQuantity(0)
{
	static ConstructorHelpers::FClassFinder<UNPCShopNeedItemInfo> NeedItemInfoBPClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Widget/NPC/WBP_Shop_NeedItemInfo");

	if (NeedItemInfoBPClass.Succeeded())
	{
		NeedInfoClass = NeedItemInfoBPClass.Class;
	}
}



void UNPCShopConfirmInteract::NativeConstruct()
{
	Super::NativeConstruct();

	ItemQuantityEditableText->OnTextChanged.AddUniqueDynamic(this, &UNPCShopConfirmInteract::OnItemQuantityTextUpdated);
	Button_InteractConfirm->OnClicked.AddUniqueDynamic(this, &UNPCShopConfirmInteract::BroadcastConfirmDelegate);
	Button_CancelConfirm->OnClicked.AddUniqueDynamic(this, &UNPCShopConfirmInteract::RemoveFromParent);
	Button_AddQuantity->OnClicked.AddUniqueDynamic(this, &UNPCShopConfirmInteract::AddQuantity);
	Button_SubQuantity->OnClicked.AddUniqueDynamic(this, &UNPCShopConfirmInteract::SubQuantity);

	ItemDetailsWidget->SetVisibility(ESlateVisibility::Collapsed);
}



void UNPCShopConfirmInteract::NativeDestruct()
{
	Super::NativeDestruct();

	ItemQuantityEditableText->OnTextChanged.RemoveAll(this);
	Button_InteractConfirm->OnClicked.RemoveAll(this);
	Button_CancelConfirm->OnClicked.RemoveAll(this);
	Button_AddQuantity->OnClicked.RemoveAll(this);
	Button_SubQuantity->OnClicked.RemoveAll(this);

	for (UWidget* ChildWidget : VerticalBox_NeedItems->GetAllChildren())
	{
		ChildWidget->RemoveFromParent();
	}
}



void UNPCShopConfirmInteract::UpdateInfo(UNPCShopItemSlot* TargetSlot)
{
	if (!IsValid(TargetSlot) || !IsValid(TargetSlot->ItemHas)) return;

	ItemToInteract = TargetSlot->ItemHas;
	MaxQuantity = TargetSlot->ItemCount;
	InteractType = TargetSlot->InteractType;

	// 아이콘 업데이트
	FSlateBrush NewBrush = ItemToInteract->ItemIcon;
	NewBrush.ImageSize = FVector2D(128.0f, 128.0f);
	Image_ItemIcon->SetBrush(NewBrush);

	FString InteractText;

	switch (InteractType)
	{
		case EShopInteractType::BUY:
			InteractText = TEXT("구매");
			break;
		case EShopInteractType::SELL:
			InteractText = TEXT("판매");
			break;
		case EShopInteractType::CRAFT:
			InteractText = TEXT("제작");
			break;
		default:
		case EShopInteractType::NONE:
			break;
	}

	InteractConfirmButtonText->SetText(FText::FromString(InteractText));



	UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();
	UGameInstanceBase* GIBase = GetGameInstance<UGameInstanceBase>();

	// 초기 필요 돈 정보 업데이트
	if (AssetManager.IsValid() && IsValid(GIBase))
	{
		NeedInfo_Money->UpdateInfo(AssetManager.LoadItem(GIBase->GetItemByTypeAndName(UGameAssetManager::ItemType_Material,
			TEXT("Item_Money_Gold_Dummy"))),
			FMath::RoundToInt(ItemToInteract->ItemPrice * EnteredQuantity *
				((InteractType == EShopInteractType::SELL) ? 0.2f : 1.0f)));

		NeedInfo_Money->MouseEnterDelegate.AddUniqueDynamic(this, &UNPCShopConfirmInteract::ShowItemDetailsWidget);
		NeedInfo_Money->MouseLeaveDelegate.AddUniqueDynamic(this, &UNPCShopConfirmInteract::HideItemDetailsWidget);
	}



	// 제작인 경우, 필요 아이템 정보 VerticalBox_NeedItems에 넣기
	if (TargetSlot->InteractType == EShopInteractType::CRAFT)
	{
		ShopSlotInteractStrategy* InteractStrategy = TargetSlot->GetInteractStrategy();

		if (ShopSlotCraftStrategy* CraftStrategy = reinterpret_cast<ShopSlotCraftStrategy*>(InteractStrategy))
		{
			SetCraftNeedItems(CraftStrategy->NeedItems);
		}
	}

	// 초기 수량 설정
	SetQuantity(1);
}



void UNPCShopConfirmInteract::OnItemQuantityTextUpdated(const FText& QuantityText)
{
	// 숫자가 아니라면 강제로 0으로 변경
	if (!QuantityText.IsNumeric()) SetQuantity(0);

	// 입력된 숫자 갱신
	SetQuantity(FMath::Min(FCString::Atoi(*QuantityText.ToString()), MaxQuantity));
}



void UNPCShopConfirmInteract::BroadcastConfirmDelegate()
{
	OnConfirmDelegate.Broadcast(this, EnteredQuantity);
}



void UNPCShopConfirmInteract::AddQuantity()
{
	SetQuantity(FMath::Min(EnteredQuantity + 1, MaxQuantity));
}



void UNPCShopConfirmInteract::SubQuantity()
{
	SetQuantity(FMath::Max(EnteredQuantity - 1, 0));
}



void UNPCShopConfirmInteract::SetQuantity(int32 NewQuantity)
{
	// 판매가 아닌 경우
	if (InteractType != EShopInteractType::SELL)
	{
		float NeedPrice = NewQuantity * ItemToInteract->ItemPrice;

		float MoneyHas = GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>()->GetMoney();

		// 돈이 충분하다면 NewQuantity, 부족하다면 현재 돈으로 구매 가능한 최대 수량
		EnteredQuantity = MoneyHas >= NeedPrice ? NewQuantity : FMath::Min(MaxQuantity, FMath::FloorToInt(MoneyHas / ItemToInteract->ItemPrice));
	}
	// 판매라면 최대 수량 초과 불가능
	else
	{
		EnteredQuantity = FMath::Min(NewQuantity, MaxQuantity);
	}

	ItemQuantityEditableText->SetText(FText::AsNumber(EnteredQuantity));
	

	// 버튼 활성화 여부 결정
	Button_InteractConfirm->SetIsEnabled(EnteredQuantity > 0);

	// 필요 돈 업데이트
	NeedInfo_Money->UpdateNeedQuantity(FMath::RoundToInt(ItemToInteract->ItemPrice * EnteredQuantity *
		((InteractType == EShopInteractType::SELL) ? 0.2f : 1.0f)));



	if (InteractType != EShopInteractType::CRAFT) return;

	// 필요 재료들 수량 업데이트
	UGISS_InteractableNpcData* NpcDataSS = GetGameInstance()->GetSubsystem<UGISS_InteractableNpcData>();
	UGISS_PlayerInventory* InventorySS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();
	UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();

	TArray<FCraftNeedItem> NeedItemArray = NpcDataSS->GetCraftNeedItems(ItemToInteract->InternalName);
	TArray<UWidget*> NeedItemWidgetArray = VerticalBox_NeedItems->GetAllChildren();

	for (int i = 0; i < NeedItemWidgetArray.Num(); i++)
	{
		if (UNPCShopNeedItemInfo* NeedItemInfoWidget = Cast<UNPCShopNeedItemInfo>(NeedItemWidgetArray[i]))
		{
			int32 FinalQuantity = EnteredQuantity * NeedItemArray[i].NeedQuantity;

			NeedItemInfoWidget->UpdateNeedQuantity(FinalQuantity);

			UItemBase* LoadedItem = AssetManager.LoadItem(NeedItemArray[i].NeedItem);

			// 필요 수량보다 보유량이 적거나 보유한 적이 없다면 색상을 빨간색으로 변경, 충분하다면 하얀색으로 변경
			NeedItemInfoWidget->SetQuantityTextColorToRed(!InventorySS->InventoryMap.Contains(LoadedItem) || InventorySS->InventoryMap[LoadedItem].Quantity < FinalQuantity);
		}
	}
}



void UNPCShopConfirmInteract::SetCraftNeedItems(const TArray<FCraftNeedItem>& NeedItems)
{
	for (const FCraftNeedItem& NeedItem : NeedItems)
	{
		UNPCShopNeedItemInfo* NeedItemInfoWidget = CreateWidget<UNPCShopNeedItemInfo>(this, NeedInfoClass);

		UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();
		UItemBase* LoadedItem = AssetManager.LoadItem(NeedItem.NeedItem);

		if (IsValid(LoadedItem))
		{
			NeedItemInfoWidget->UpdateInfo(LoadedItem, NeedItem.NeedQuantity);

			NeedItemInfoWidget->MouseEnterDelegate.AddUniqueDynamic(this, &UNPCShopConfirmInteract::ShowItemDetailsWidget);
			NeedItemInfoWidget->MouseLeaveDelegate.AddUniqueDynamic(this, &UNPCShopConfirmInteract::HideItemDetailsWidget);
		}

		VerticalBox_NeedItems->AddChildToVerticalBox(NeedItemInfoWidget);
	}
}



void UNPCShopConfirmInteract::ShowItemDetailsWidget(UItemBase* Item)
{
	if (!IsValid(Item)) return;

	ItemDetailsWidget->UpdateItem(Item);
	ItemDetailsWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}



void UNPCShopConfirmInteract::HideItemDetailsWidget(UItemBase* Item)
{
	ItemDetailsWidget->SetVisibility(ESlateVisibility::Collapsed);
}
