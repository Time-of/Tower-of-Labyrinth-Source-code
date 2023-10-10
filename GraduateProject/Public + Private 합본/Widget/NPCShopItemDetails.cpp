
#include "Widget/NPCShopItemDetails.h"

#include "GameAssetManager.h"
#include "GameInstance/GameInstanceBase.h"
#include "Item/ItemBase.h"
#include "NPC/DataTable/ShopCraftNeedItemsTableRow.h"
#include "Widget/NPCShopNeedItemInfo.h"

#include "Components/Border.h"
#include "Components/RichTextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"



UNPCShopItemDetails::UNPCShopItemDetails(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	NeedItemsScrollBox(nullptr),
	NeedInfo_Money(nullptr), VerticalBox_NeedItems(nullptr),
	InteractTextBlock(nullptr), InteractBorder(nullptr),
	NeedInfoClass(nullptr), AutoScrollElapsedTime(0.0f)
{
	static ConstructorHelpers::FClassFinder<UNPCShopNeedItemInfo> NeedItemInfoBPClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Widget/NPC/WBP_Shop_NeedItemInfo");

	if (NeedItemInfoBPClass.Succeeded())
	{
		NeedInfoClass = NeedItemInfoBPClass.Class;
	}
}



void UNPCShopItemDetails::NativeConstruct()
{
	Super::NativeConstruct();

	OnVisibilityChanged.AddUniqueDynamic(this, &UNPCShopItemDetails::AfterVisibilityChanged);
}



void UNPCShopItemDetails::NativeDestruct()
{
	Super::NativeDestruct();

	RemoveCraftNeedItems();

	OnVisibilityChanged.RemoveDynamic(this, &UNPCShopItemDetails::AfterVisibilityChanged);
}



void UNPCShopItemDetails::UpdateItem(const UItemBase* Item)
{
	TArray<FCraftNeedItem> DefaultEmptyArray;
	UpdateItem(Item, DefaultEmptyArray);
}



void UNPCShopItemDetails::UpdateItem(const UItemBase* Item, const TArray<FCraftNeedItem>& CraftNeedItemArray)
{
	if (!IsValid(Item)) return;

	UNPCShopItemDetails_Simplified::UpdateItem(Item);

	UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();
	UGameInstanceBase* GIBase = GetGameInstance<UGameInstanceBase>();

	// 초기 필요 돈 정보 업데이트
	if (AssetManager.IsValid() && IsValid(GIBase))
	{
		NeedInfo_Money->UpdateInfo(AssetManager.LoadItem(GIBase->GetItemByTypeAndName(UGameAssetManager::ItemType_Material,
			TEXT("Item_Money_Gold_Dummy"))),
			Item->ItemPrice);
	}

	RemoveCraftNeedItems();
	SetCraftNeedItems(CraftNeedItemArray);

	SetAdditionalDescriptionText(Item);
}



void UNPCShopItemDetails::UpdateTextUnknownItem()
{
	ItemNameText->SetText(FText::FromString(TEXT("???")));
	ItemTypeText->SetText(FText::FromString(TEXT("Unknown")));
	ItemDescriptionText->SetText(FText::FromString(TEXT("???")));
}



void UNPCShopItemDetails::UpdateMoneyText(float NewMoney)
{
	NeedInfo_Money->UpdateNeedQuantity(FMath::RoundToInt(NewMoney));
}



void UNPCShopItemDetails::SetCraftNeedItems(const TArray<FCraftNeedItem>& NeedItems)
{
	for (const FCraftNeedItem& NeedItem : NeedItems)
	{
		UNPCShopNeedItemInfo* NeedItemInfoWidget = CreateWidget<UNPCShopNeedItemInfo>(this, NeedInfoClass);

		UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();
		UItemBase* LoadedItem = AssetManager.LoadItem(NeedItem.NeedItem);

		if (IsValid(LoadedItem))
		{
			NeedItemInfoWidget->UpdateInfo(LoadedItem, NeedItem.NeedQuantity);
		}

		VerticalBox_NeedItems->AddChildToVerticalBox(NeedItemInfoWidget);
	}
}



void UNPCShopItemDetails::RemoveCraftNeedItems()
{
	for (UWidget* ChildWidget : VerticalBox_NeedItems->GetAllChildren())
	{
		ChildWidget->RemoveFromParent();
	}
}



void UNPCShopItemDetails::SetInteractTextByType(EShopInteractType Type)
{
	FText InteractText;

	switch (Type)
	{
		case EShopInteractType::BUY:
			InteractText = FText::FromString(FString(TEXT("구매")));
			break;
		case EShopInteractType::SELL:
			InteractText = FText::FromString(FString(TEXT("판매")));
			break;
		case EShopInteractType::CRAFT:
			InteractText = FText::FromString(FString(TEXT("제작")));
			break;
		default:
		case EShopInteractType::NONE:
			InteractText = FText::FromString(FString(TEXT("")));
			break;
	}

	InteractTextBlock->SetText(InteractText);
}



void UNPCShopItemDetails::SetInteractTextVisibility(ESlateVisibility NewVisibility)
{
	InteractTextBlock->SetVisibility(NewVisibility);
	InteractBorder->SetVisibility(NewVisibility);
}



void UNPCShopItemDetails::AfterVisibilityChanged(ESlateVisibility NewVisibility)
{
	GetWorld()->GetTimerManager().ClearTimer(AutoScrollTimerHandle);
	NeedItemsScrollBox->SetScrollOffset(0.0f);

	AutoScrollElapsedTime = 0.0f;

	if (NewVisibility == ESlateVisibility::Collapsed) return;

	GetWorld()->GetTimerManager().SetTimer(AutoScrollTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			StartAutoScroll();
		}),
		0.5f, false);
}



void UNPCShopItemDetails::StartAutoScroll()
{
	if (NeedItemsScrollBox->GetScrollOffsetOfEnd() > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoScrollTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				if (AutoScrollElapsedTime < 1.0f)
				{
					AutoScrollElapsedTime += 0.008f;
					NeedItemsScrollBox->SetScrollOffset(
						FMath::Lerp<float>(0.0f, NeedItemsScrollBox->GetScrollOffsetOfEnd(), FMath::Min(AutoScrollElapsedTime, 1.0f)));
				}
			}), 0.016667f, true);
	}
}
