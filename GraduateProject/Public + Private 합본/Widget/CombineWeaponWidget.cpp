
#include "Widget/CombineWeaponWidget.h"

#include "GameAssetManager.h"
#include "GameInstance/GameInstanceBase.h"
#include "GameInstance/GISS_PlayerInventory.h"
#include "GameInstance/GISS_PlayerMoney.h"
#include "Item/Item_WeaponBase.h"
#include "Player/PlayerCharacterBase.h"
#include "Widget/InventorySlot.h"
#include "Widget/NPCShopItemDetails.h"

#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"



UCombineWeaponWidget::UCombineWeaponWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	DefaultInventoryRowCount(3),
	DefaultInventoryColumnCount(4),
	CurrentInventoryRowCount(3),
	CombineButton(nullptr),
	CombineResultItemSlot(nullptr),
	CombineMaterialItemSlot_Left(nullptr),
	CombineMaterialItemSlot_Right(nullptr),
	UniformGrid_Inventory(nullptr),
	ItemDetailsWidget(nullptr),
	InventorySlotClass(nullptr),
	InventoryLastIndex(0),
	SelectedLeftSlotRef(nullptr),
	SelectedRightSlotRef(nullptr)
{
	static ConstructorHelpers::FClassFinder<UInventorySlot> DefaultItemSlotClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Widget/Inventory/WBP_Inventory_ItemSlot");

	if (DefaultItemSlotClass.Succeeded())
	{
		InventorySlotClass = DefaultItemSlotClass.Class;
	}
}



void UCombineWeaponWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentInventoryRowCount = DefaultInventoryRowCount;

	CreateInventorySlots(CurrentInventoryRowCount, DefaultInventoryColumnCount);
	
	LoadWeaponsFromInventory();

	if (UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>())
	{
		InvSS->OnInventoryChangedDelegate.AddUniqueDynamic(this, &UCombineWeaponWidget::OnInventoryUpdated);
	}

	BindCombineDelegates();
}



void UCombineWeaponWidget::NativeDestruct()
{
	Super::NativeDestruct();

	RemoveInventorySlots();

	if (UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>())
	{
		InvSS->OnInventoryChangedDelegate.RemoveDynamic(this, &UCombineWeaponWidget::OnInventoryUpdated);
	}

	UnbindCombineDelegates();

	// 결과 아이템이 남겨진 채로 나가는 경우의 처리
	OnResultSlotClicked(CombineResultItemSlot);
}



void UCombineWeaponWidget::CreateInventorySlots(uint8 Row, uint8 Column)
{
	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Column; j++)
		{
			UInventorySlot* CreatedSlotWidget = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotClass);

			UniformGrid_Inventory->AddChildToUniformGrid(CreatedSlotWidget, i, j);

			BindMouseEnterLeaveDelegatesOnSlot(CreatedSlotWidget);

			CreatedSlotWidget->OnClickedDelegate.AddUniqueDynamic(this, &UCombineWeaponWidget::SelectInventoryItem);
		}
	}
}



void UCombineWeaponWidget::RemoveInventorySlots()
{
	for (UWidget* Child : UniformGrid_Inventory->GetAllChildren())
	{
		if (UInventorySlot* SlotWidget = Cast<UInventorySlot>(Child))
		{
			UnbindMouseEnterLeaveDelegatesOnSlot(SlotWidget);

			SlotWidget->OnClickedDelegate.RemoveDynamic(this, &UCombineWeaponWidget::SelectInventoryItem);
		}

		Child->RemoveFromParent();
	}
}



void UCombineWeaponWidget::LoadWeaponsFromInventory()
{
	InventoryLastIndex = 0;

	UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();

	UItem_WeaponBase* EquippedWeapon = nullptr;

	APlayerCharacterBase* PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacterBase>();

	if (IsValid(PlayerCharacterRef))
	{
		EquippedWeapon = PlayerCharacterRef->GetEquippedWeapon();
	}

	// 장착한 무기인 경우 표시하지 않기 위해 사용하는 변수
	bool bWasEquippedWeaponSkipped = false;


	for (const auto& ItemInMap : InvSS->InventoryMap)
	{
		int32 ItemQuantity = ItemInMap.Value.Quantity;
		int32 ItemMaxQuantity = ItemInMap.Value.MaxQuantity;

		UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();

		// 보유량이 최대치를 넘는 경우 옆 칸으로 이동하여 설정
		while (ItemQuantity > 0)
		{
			// 장착한 무기일 때 스킵되지 않았다면 1회에 한해 스킵
			// (장착한 무기와 같은 종류의 아이템이 2개 이상 있는 경우의 처리)
			if (!bWasEquippedWeaponSkipped
				&& IsValid(EquippedWeapon) 
				&& ItemInMap.Key == EquippedWeapon)
			{
				--ItemQuantity;
				bWasEquippedWeaponSkipped = true;
				continue;
			}

			// 무기인 경우에만 인벤토리 슬롯에 할당
			if (ItemInMap.Key->ItemType == AssetManager.ItemType_Weapon)
			{
				SetInventorySlotByIndex(ItemInMap.Key,
					ItemQuantity > ItemMaxQuantity ? ItemMaxQuantity : ItemQuantity,
					InventoryLastIndex++);
			}

			CreateNewSlotIfOutOfIndex();

			ItemQuantity -= ItemMaxQuantity;
		}

		// 0개 보유 중인(즉, 있었는데 없어진) 아이템에 관한 설정
		if (ItemInMap.Value.Quantity == 0)
		{
			SetInventorySlotByIndex(ItemInMap.Key, 0, InventoryLastIndex++);
		}
	}
}



void UCombineWeaponWidget::SetInventorySlotByIndex(UItemBase* ItemToSet, int32 NewItemCount, int32 Index)
{
	if (UWidget* ChildWidget = UniformGrid_Inventory->GetChildAt(Index))
	{
		if (UInventorySlot* SlotWidget = Cast<UInventorySlot>(ChildWidget))
		{
			SlotWidget->UpdateItemSlot(ItemToSet, NewItemCount);
		}
	}
}



void UCombineWeaponWidget::OnInventoryUpdated(UItemBase* Item, int32 NewItemCount, int32 DeltaQuantity)
{
	UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();

	int32 ItemQuantity = DeltaQuantity;
	int32 ItemMaxQuantity = Item->MaxCount;

	if (InvSS->InventoryMap.Contains(Item))
	{
		// 개수가 감소 (-)
		if (ItemQuantity < 0)
		{
			while (ItemQuantity < 0)
			{
				UpdateSlotOnItemRemoved(Item, ItemQuantity, ItemMaxQuantity);

				ItemQuantity += ItemMaxQuantity;
			}
		}
		// 개수가 증가 (+)
		else
		{
			UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();

			// 보유량이 최대치를 넘는 경우 옆 칸으로 이동하여 설정
			while (ItemQuantity > 0)
			{
				// 무기인 경우에만 인벤토리 슬롯에 할당
				if (Item->ItemType == AssetManager.ItemType_Weapon)
				{
					UpdateSlotOnItemAdded(InventoryLastIndex, Item, ItemQuantity, ItemMaxQuantity);
				}

				CreateNewSlotIfOutOfIndex();

				ItemQuantity -= ItemMaxQuantity;
			}
		}
	}
	// 새로 획득한 아이템인 경우, 새로 아이템을 설정
	else
	{
		SetInventorySlotByIndex(Item, ItemQuantity, InventoryLastIndex++);
	}
}



void UCombineWeaponWidget::UpdateSlotOnItemRemoved(UItemBase* Item, int32& ItemQuantity, int32 ItemMaxQuantity)
{
	for (const auto& WidgetSlot : UniformGrid_Inventory->GetAllChildren())
	{
		UInventorySlot* InvSlot = Cast<UInventorySlot>(WidgetSlot);

		uint32 DeltaQuantity = FMath::Abs(ItemQuantity);

		// 해당하는 아이템 찾기
		if (IsValid(InvSlot) && InvSlot->ItemHas == Item)
		{
			// 이미 빈 슬롯이라면 다음 루프로 진행
			if (InvSlot->ItemCount <= 0) continue;
			// 해당 아이템 수량만큼 뺐을 때, 수량이 남는(1 이상인) 경우
			// 즉, 현재 아이템 수량이 '빼고자 하는 양'보다 많은 경우
			else if (InvSlot->ItemCount - DeltaQuantity > 0)
			{
				InvSlot->UpdateItemSlot(Item, InvSlot->ItemCount - DeltaQuantity);

				return;
			}
			// 뺐을 때 해당 슬롯이 0 이하가 되는 경우 0으로 만들고 다음 루프로 진행
			else
			{
				ItemQuantity += InvSlot->ItemCount;
				InvSlot->UpdateItemSlot(Item, 0);

				continue;
			}
		}
	}
}



void UCombineWeaponWidget::UpdateSlotOnItemAdded(uint32& SlotIndex, UItemBase* Item, int32& ItemQuantity, int32 ItemMaxQuantity)
{
	for (const auto& WidgetSlot : UniformGrid_Inventory->GetAllChildren())
	{
		UInventorySlot* InvSlot = Cast<UInventorySlot>(WidgetSlot);

		bool bWasEmptySlot = !IsValid(InvSlot->ItemHas);

		if (IsValid(InvSlot) && (InvSlot->ItemHas == Item || bWasEmptySlot))
		{
			// 만약 해당 슬롯이 이미 꽉 찬 경우 다음 루프로 진행
			if (InvSlot->ItemCount >= ItemMaxQuantity) continue;
			// 만약 현재 슬롯 아이템 수량을 채웠을 때 최대량을 초과하지 않는 경우 (예: 2개 <= 99개 - 97개)
			else if (ItemQuantity <= ItemMaxQuantity - InvSlot->ItemCount)
			{
				InvSlot->UpdateItemSlot(Item, ItemQuantity + InvSlot->ItemCount);

				// 빈 슬롯을 채운 경우에만 슬롯 인덱스 증가시키기
				if (bWasEmptySlot) ++SlotIndex;

				UE_LOG(LogTemp, Warning, TEXT("초과X 슬롯인덱스: %d"), InventoryLastIndex);

				CreateNewSlotIfOutOfIndex();

				return;
			}
			// 채웠을 때 초과하는 경우 남은 양을 채우고 다음 루프로 진행
			else
			{
				ItemQuantity -= ItemMaxQuantity - InvSlot->ItemCount;
				InvSlot->UpdateItemSlot(Item, ItemMaxQuantity);

				++SlotIndex;

				UE_LOG(LogTemp, Warning, TEXT("초과O 슬롯인덱스: %d"), InventoryLastIndex);

				CreateNewSlotIfOutOfIndex();

				continue;
			}
		}
	}
}



void UCombineWeaponWidget::ShowItemDetailsWidget(UItemBase* Item)
{
	if (!IsValid(Item)) return;

	ItemDetailsWidget->UpdateItem(Item);

	ItemDetailsWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}



void UCombineWeaponWidget::HideItemDetailsWidget(UItemBase* Item)
{
	ItemDetailsWidget->SetVisibility(ESlateVisibility::Collapsed);
}



void UCombineWeaponWidget::SelectInventoryItem(UInventorySlot* InvSlot)
{
	if (!IsValid(InvSlot->ItemHas)) return;

	// 빈 칸이 없다면 선택 불가
	bool bOutIsLeftSlot = false;
	if (!GetEmptyMaterialSlot(bOutIsLeftSlot)) return;

	// 이미 선택한 슬롯(하이라이트된 슬롯)인 경우 선택 불가
	if (InvSlot->IsHighlighted()) return;

	SetMaterialSlot(bOutIsLeftSlot, InvSlot);
}



bool UCombineWeaponWidget::GetEmptyMaterialSlot(bool& OutIsLeftSlot)
{
	if (!IsValid(CombineMaterialItemSlot_Left->ItemHas))
	{
		OutIsLeftSlot = true;
		return true;
	}
	
	if (!IsValid(CombineMaterialItemSlot_Right->ItemHas))
	{
		OutIsLeftSlot = false;
		return true;
	}

	return false;
}



void UCombineWeaponWidget::SetMaterialSlot(bool bIsLeftSlot, UInventorySlot* InvSlot)
{
	UInventorySlot* SelectedSlot = bIsLeftSlot ?
		CombineMaterialItemSlot_Left : CombineMaterialItemSlot_Right;
	
	SelectedSlot->UpdateItemSlot(InvSlot->ItemHas, 1);

	ChangeHighlightOnInventorySlot(bIsLeftSlot, InvSlot);
}



void UCombineWeaponWidget::SetMaterialSlotToEmpty(bool bIsLeftSlot)
{
	UInventorySlot* SelectedSlot = bIsLeftSlot ?
		CombineMaterialItemSlot_Left : CombineMaterialItemSlot_Right;

	SelectedSlot->UpdateItemSlot(nullptr, 0);

	ChangeHighlightOnInventorySlot(bIsLeftSlot, nullptr);
}



void UCombineWeaponWidget::ChangeHighlightOnInventorySlot(bool bIsLeftSlot, UInventorySlot* InvSlot)
{
	UInventorySlot* SelectedSlot = bIsLeftSlot ?
		SelectedLeftSlotRef : SelectedRightSlotRef;

	if (IsValid(SelectedSlot))
	{
		SelectedSlot->ShowHighlight(false);
	}

	if (bIsLeftSlot)
	{
		if (IsValid(InvSlot))
		{
			SelectedLeftSlotRef = InvSlot;
			SelectedLeftSlotRef->ShowHighlight(true);
		}
		else
		{
			SelectedLeftSlotRef = nullptr;
		}
	}
	else
	{
		if (IsValid(InvSlot))
		{
			SelectedRightSlotRef = InvSlot;
			SelectedRightSlotRef->ShowHighlight(true);
		}
		else
		{
			SelectedRightSlotRef = nullptr;
		}
	}
}



void UCombineWeaponWidget::InternalSetLeftMaterialSlotToEmpty(UInventorySlot* InvSlot)
{
	SetMaterialSlotToEmpty(true);
}



void UCombineWeaponWidget::InternalSetRightMaterialSlotToEmpty(UInventorySlot* InvSlot)
{
	SetMaterialSlotToEmpty(false);
}



void UCombineWeaponWidget::ExecuteCombineOnCombineButtonClicked()
{
	bool bTempBoolean = false;

	// 빈 슬롯이 없어야 함. 빈 슬롯이 있다면 반환
	if (GetEmptyMaterialSlot(bTempBoolean)) return;

	// 재료 소모
	UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();
	if (InvSS->InventoryMap.Contains(SelectedLeftSlotRef->ItemHas)
		&& InvSS->InventoryMap.Contains(SelectedRightSlotRef->ItemHas)
		&& InvSS->InventoryMap[SelectedLeftSlotRef->ItemHas].Quantity > 0
		&& InvSS->InventoryMap[SelectedRightSlotRef->ItemHas].Quantity > 0)
	{
		InvSS->AddItem(SelectedLeftSlotRef->ItemHas, -1);
		InvSS->AddItem(SelectedRightSlotRef->ItemHas, -1);
	}
	// 재료가 없는데도 조합하려 하는 경우가 발생한다면 반환
	else return;

	UGameInstanceBase* GIBase = GetGameInstance<UGameInstanceBase>();

	float LeftWeight = GIBase->GetWeaponWeightInWeightedTable(CombineMaterialItemSlot_Left->ItemHas->InternalName);
	float RightWeight = GIBase->GetWeaponWeightInWeightedTable(CombineMaterialItemSlot_Right->ItemHas->InternalName);

	// 양쪽 재료 슬롯 비우기
	SetMaterialSlotToEmpty(true);
	SetMaterialSlotToEmpty(false);

	// 이미 결과 슬롯에 아이템이 있는 경우의 처리 (사라지지 않도록)
	OnResultSlotClicked(CombineResultItemSlot);

	float AverageWeight = (LeftWeight + RightWeight) / 2;

	AddRandomWeaponToResultSlot(AverageWeight - 10.0f, AverageWeight + 10.0f);
}



void UCombineWeaponWidget::AddRandomWeaponToResultSlot(float MinWeight, float MaxWeight)
{
	UGameInstanceBase* GIBase = GetGameInstance<UGameInstanceBase>();

	UItemBase* FoundItem = GIBase->GetWeaponFromWeightedWeaponTableInWeightRange(MinWeight, MaxWeight);

	if (!IsValid(FoundItem))
	{
		UE_LOG(LogTemp, Warning, TEXT("CombineWeaponWidget: GameInstanceBase에서 무작위 무기를 찾을 수 없음!!"));
		return;
	}
	CombineResultItemSlot->UpdateItemSlot(FoundItem, 1);

	OnResultSlotItemAdded();
}



void UCombineWeaponWidget::OnResultSlotClicked(UInventorySlot* InvSlot)
{
	if (!IsValid(InvSlot->ItemHas)) return;

	UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();
	InvSS->AddItem(InvSlot->ItemHas, 1);
	
	InvSlot->UpdateItemSlot(nullptr, 0);
}



void UCombineWeaponWidget::BindCombineDelegates()
{
	CombineMaterialItemSlot_Left->OnClickedDelegate.AddUniqueDynamic(this,
		&UCombineWeaponWidget::InternalSetLeftMaterialSlotToEmpty);

	CombineMaterialItemSlot_Right->OnClickedDelegate.AddUniqueDynamic(this,
		&UCombineWeaponWidget::InternalSetRightMaterialSlotToEmpty);

	CombineButton->OnClicked.AddUniqueDynamic(this,
		&UCombineWeaponWidget::ExecuteCombineOnCombineButtonClicked);

	CombineResultItemSlot->OnClickedDelegate.AddUniqueDynamic(this,
		&UCombineWeaponWidget::OnResultSlotClicked);

	BindMouseEnterLeaveDelegatesOnSlot(CombineMaterialItemSlot_Left);
	BindMouseEnterLeaveDelegatesOnSlot(CombineMaterialItemSlot_Right);
	BindMouseEnterLeaveDelegatesOnSlot(CombineResultItemSlot);
}



void UCombineWeaponWidget::UnbindCombineDelegates()
{
	CombineMaterialItemSlot_Left->OnClickedDelegate.RemoveDynamic(this,
		&UCombineWeaponWidget::InternalSetLeftMaterialSlotToEmpty);

	CombineMaterialItemSlot_Right->OnClickedDelegate.RemoveDynamic(this,
		&UCombineWeaponWidget::InternalSetRightMaterialSlotToEmpty);

	CombineButton->OnClicked.RemoveDynamic(this,
		&UCombineWeaponWidget::ExecuteCombineOnCombineButtonClicked);

	CombineResultItemSlot->OnClickedDelegate.RemoveDynamic(this,
		&UCombineWeaponWidget::OnResultSlotClicked);

	UnbindMouseEnterLeaveDelegatesOnSlot(CombineMaterialItemSlot_Left);
	UnbindMouseEnterLeaveDelegatesOnSlot(CombineMaterialItemSlot_Right);
	UnbindMouseEnterLeaveDelegatesOnSlot(CombineResultItemSlot);
}



void UCombineWeaponWidget::BindMouseEnterLeaveDelegatesOnSlot(UInventorySlot* InvSlot)
{
	InvSlot->MouseEnterDelegate.AddUniqueDynamic(this, &UCombineWeaponWidget::ShowItemDetailsWidget);
	InvSlot->MouseLeaveDelegate.AddUniqueDynamic(this, &UCombineWeaponWidget::HideItemDetailsWidget);
}



void UCombineWeaponWidget::UnbindMouseEnterLeaveDelegatesOnSlot(UInventorySlot* InvSlot)
{
	InvSlot->MouseEnterDelegate.RemoveDynamic(this, &UCombineWeaponWidget::ShowItemDetailsWidget);
	InvSlot->MouseLeaveDelegate.RemoveDynamic(this, &UCombineWeaponWidget::HideItemDetailsWidget);
}



void UCombineWeaponWidget::CreateNewSlotIfOutOfIndex()
{
	if (InventoryLastIndex >= static_cast<uint32>(CurrentInventoryRowCount * DefaultInventoryColumnCount) - 1)
	{
		// 새로 행을 하나 추가
		for (int i = 0; i < DefaultInventoryColumnCount; i++)
		{
			UInventorySlot* CreatedSlotWidget = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotClass);

			UniformGrid_Inventory->AddChildToUniformGrid(CreatedSlotWidget, CurrentInventoryRowCount, i);

			BindMouseEnterLeaveDelegatesOnSlot(CreatedSlotWidget);

			CreatedSlotWidget->OnClickedDelegate.AddUniqueDynamic(this, &UCombineWeaponWidget::SelectInventoryItem);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("CombineWeaponWidget: 인벤토리 슬롯이 부족해 %d 행 생성, 현재 인덱스: %d"),
			CurrentInventoryRowCount, InventoryLastIndex);

		++CurrentInventoryRowCount;
	}
}
