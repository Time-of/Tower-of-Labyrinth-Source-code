
#include "Widget/PlayerInventoryWidget.h"

#include "CharacterBase.h"
#include "GameAssetManager.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "GameInstance/GISS_PlayerInventory.h"
#include "GameInstance/GISS_PlayerMoney.h"
#include "GameInstance/GISS_ShuffleLevel.h"
#include "GameInstance/GameInstanceBase.h"
#include "Item/ItemBase.h"
#include "Item/Item_ArmorBase.h"
#include "Item/Item_CharacterSkillBase.h"
#include "Item/Item_WeaponBase.h"
#include "Player/PlayerCharacterBase.h"
#include "Widget/InventorySkillDetailsWidget.h"
#include "Widget/InventorySkillSlotWidget.h"
#include "Widget/InventorySlot.h"
#include "Widget/NPCShopItemDetails.h"
#include "Widget/NPCShopNeedItemInfo.h"

#include "Camera/CameraComponent.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"



UPlayerInventoryWidget::UPlayerInventoryWidget(const FObjectInitializer& ObjectInitializer) : UInteractableNPCWidgetBase(ObjectInitializer),
	InventorySlotRow(5), InventorySlotColumn(5),
	OriginalCameraRef(nullptr), ViewCameraRef(nullptr),
	HpText(nullptr), StrText(nullptr), DefText(nullptr),
	EquippedArmorItemImage(nullptr),
	UniformGrid_Weapon(nullptr),
	UniformGrid_Armor(nullptr),
	UniformGrid_Misc(nullptr),
	Button_EquipItem(nullptr), EquipItemButtonText(nullptr),
	ItemDetailsWidget(nullptr), SkillDetailsWidget(nullptr),
	NeedInfo_Money(nullptr),
	SkillSlot_Q(nullptr), SkillSlot_E(nullptr),
	SkillSlot_F(nullptr), SkillSlot_RM(nullptr),
	SelectedSlotRef(nullptr),
	EquippedWeaponSlotRef(nullptr), EquippedArmorSlotRef(nullptr),
	WeaponSlotIndex(0), ArmorSlotIndex(0), MiscSlotIndex(0)
{
	static ConstructorHelpers::FClassFinder<UInventorySkillSlotWidget> SkillSlotWBPClass(TEXT("/Game/SungSoo_Workspace/Blueprints/Widget/Inventory/WBP_Inventory_SkillSlot"));

	if (SkillSlotWBPClass.Succeeded())
	{
		SkillSlotWidgetClass = SkillSlotWBPClass.Class;
	}
}



void UPlayerInventoryWidget::NativeConstruct()
{
	UInteractableNPCWidgetBase::NativeConstruct();

	ItemDetailsWidget->SetInteractTextVisibility(ESlateVisibility::Collapsed);
	Button_EquipItem->SetVisibility(ESlateVisibility::Collapsed);
	Button_EquipItem->OnClicked.AddUniqueDynamic(this, &UPlayerInventoryWidget::OnEquipButtonClicked);
	HideItemDetailsWidget(nullptr);
	HideSkillDetailsWidget(nullptr);

	UpdateStatTexts();
	SelectSlot(nullptr);

	if (ACharacterBase* PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn<ACharacterBase>())
	{
		PlayerCharacterRef->OnHealthChangedDelegate.AddUniqueDynamic(this, &UPlayerInventoryWidget::UpdateHealthText);
	}

	if (UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>())
	{
		InvSS->OnInventoryChangedDelegate.AddUniqueDynamic(this, &UPlayerInventoryWidget::OnInventoryUpdated);
	}

	UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();
	UGameInstanceBase* GIBase = GetGameInstance<UGameInstanceBase>();
	UGISS_PlayerMoney* MoneySS = GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>();

	// 초기 돈 정보 업데이트
	if (AssetManager.IsValid() && IsValid(GIBase))
	{
		NeedInfo_Money->UpdateInfo(AssetManager.LoadItem(GIBase->GetItemByTypeAndName(UGameAssetManager::ItemType_Material,
			TEXT("Item_Money_Gold_Dummy"))),
			MoneySS->GetMoney());

		MoneySS->OnMoneyChanged.AddUniqueDynamic(this, &UPlayerInventoryWidget::UpdateMoneyTextOnMoneyChanged);
	}

	LoadDefaultSkillSlots();

	LoadInventoryData();
}



void UPlayerInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();

	SelectedSlotRef = nullptr;
	EquippedWeaponSlotRef = nullptr;
	EquippedArmorSlotRef = nullptr;

	Button_EquipItem->OnClicked.RemoveDynamic(this, &UPlayerInventoryWidget::OnEquipButtonClicked);

	UWorld* WorldRef = GetWorld();
	APlayerController* PCRef = IsValid(WorldRef) ? WorldRef->GetFirstPlayerController() : nullptr;
	APlayerCharacterBase* PlayerCharacterRef = IsValid(PCRef) ? PCRef->GetPawn<APlayerCharacterBase>() : nullptr;

	if (IsValid(PlayerCharacterRef))
	{
		PlayerCharacterRef->OnHealthChangedDelegate.RemoveDynamic(this, &UPlayerInventoryWidget::UpdateHealthText);
		PlayerCharacterRef->OnSkillEquipmentChanged.RemoveDynamic(this, &UPlayerInventoryWidget::UpdateSkillSlot);
	}

	if (UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>())
	{
		InvSS->OnInventoryChangedDelegate.RemoveDynamic(this, &UPlayerInventoryWidget::OnInventoryUpdated);
	}

	if (UGISS_PlayerMoney* MoneySS = GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>())
	{
		MoneySS->OnMoneyChanged.RemoveDynamic(this, &UPlayerInventoryWidget::UpdateMoneyTextOnMoneyChanged);
	}

	ClearSkillSlots();
	RemoveItemSlots(UniformGrid_Weapon);
	RemoveItemSlots(UniformGrid_Armor);
	RemoveItemSlots(UniformGrid_Misc);
}



FReply UPlayerInventoryWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	TArray<FInputActionKeyMapping> OutMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName(TEXT("Escape"), OutMappings);
	UInputSettings::GetInputSettings()->GetActionMappingByName(TEXT("Inventory"), OutMappings);

	if (!OutMappings.IsValidIndex(0)) return FReply::Unhandled();

	// Escape 또는 Inventory 입력을 받은 경우
	if (InKeyEvent.GetKey() == OutMappings[0].Key || InKeyEvent.GetKey() == OutMappings[1].Key)
	{
		RemoveThisFromParent();

		return FReply::Handled();
	}

	return FReply::Unhandled();
}



void UPlayerInventoryWidget::AddThisToViewport(UCameraComponent* OriginalCamera, UCameraComponent* ViewCamera, AActor* PlayerCharacter)
{
	AddToViewport(12);

	APlayerController* PlayerControllerRef = GWorld->GetFirstPlayerController();

	if (IsValid(PlayerControllerRef))
	{
		OriginalCameraRef = OriginalCamera;
		ViewCameraRef = ViewCamera;

		PlayerControllerRef->SetShowMouseCursor(true);

		if (IsValid(OriginalCameraRef) && IsValid(ViewCameraRef))
		{
			OriginalCameraRef->SetActive(false);
			ViewCameraRef->SetActive(true);
		}

		PlayerControllerRef->SetViewTargetWithBlend(PlayerCharacter, 0.2f, VTBlend_Cubic, 3.0f);

		FInputModeUIOnly UIOnlyInputMode;
		UIOnlyInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerControllerRef->SetInputMode(UIOnlyInputMode);
	}
}



void UPlayerInventoryWidget::SetupItemSlots(UUniformGridPanel* UniformGrid, TSubclassOf<class UInventorySlot> InventorySlotClass)
{
	for (int i = 0; i < InventorySlotRow; i++)
	{
		for (int j = 0; j < InventorySlotColumn; j++)
		{
			UInventorySlot* CreatedSlotWidget = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotClass);

			UniformGrid->AddChildToUniformGrid(CreatedSlotWidget, i, j);

			CreatedSlotWidget->MouseEnterDelegate.AddUniqueDynamic(this, &UPlayerInventoryWidget::ShowItemDetailsWidget);
			CreatedSlotWidget->MouseLeaveDelegate.AddUniqueDynamic(this, &UPlayerInventoryWidget::HideItemDetailsWidget);

			CreatedSlotWidget->OnClickedDelegate.AddUniqueDynamic(this, &UPlayerInventoryWidget::SelectSlot);
		}
	}
}



void UPlayerInventoryWidget::RemoveItemSlots(UUniformGridPanel* UniformGrid)
{
	for (UWidget* Child : UniformGrid->GetAllChildren())
	{
		if (UInventorySlot* SlotWidget = Cast<UInventorySlot>(Child))
		{
			SlotWidget->MouseEnterDelegate.RemoveDynamic(this, &UPlayerInventoryWidget::ShowItemDetailsWidget);
			SlotWidget->MouseLeaveDelegate.RemoveDynamic(this, &UPlayerInventoryWidget::HideItemDetailsWidget);

			SlotWidget->OnClickedDelegate.RemoveDynamic(this, &UPlayerInventoryWidget::SelectSlot);
		}

		Child->RemoveFromParent();
	}
}



void UPlayerInventoryWidget::ClearSkillSlots()
{
	ClearSkillSlot(SkillSlot_Q);
	ClearSkillSlot(SkillSlot_E);
	ClearSkillSlot(SkillSlot_F);
	ClearSkillSlot(SkillSlot_RM);
}



void UPlayerInventoryWidget::ClearSkillSlot(UInventorySkillSlotWidget* SkillSlot)
{
	SkillSlot->OnMouseEnteredToSlot.RemoveDynamic(this, &UPlayerInventoryWidget::ShowSkillDetailsWidget);
	SkillSlot->OnMouseLeaveFromSlot.RemoveDynamic(this, &UPlayerInventoryWidget::HideSkillDetailsWidget);
}



void UPlayerInventoryWidget::RemoveThisFromParent()
{
	if (IsValid(OriginalCameraRef) && IsValid(ViewCameraRef))
	{
		OriginalCameraRef->SetActive(true);
		ViewCameraRef->SetActive(false);
	}

	UInteractableNPCWidgetBase::RemoveThisFromParent();
}



void UPlayerInventoryWidget::LoadInventoryData()
{
	UGISS_PlayerInventory* InvRef = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();
	WeaponSlotIndex = 0;
	ArmorSlotIndex = 0;
	MiscSlotIndex = 0;

	int32 SkillSlotIndex = 0;

	UItem_WeaponBase* EquippedWeapon = nullptr;
	UItem_ArmorBase* EquippedArmor = nullptr;

	APlayerCharacterBase* PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacterBase>();

	if (IsValid(PlayerCharacterRef))
	{
		EquippedWeapon = PlayerCharacterRef->GetEquippedWeapon();
		EquippedArmor = PlayerCharacterRef->GetEquippedArmor();
	}

	// 중복되는 무기/방어구가 있는 경우 가장 첫 번째 아이템만 '장착됨 표시'를 보여주기 위한 변수
	bool bEquippedWeaponHaveShown = false;
	bool bEquippedArmorHaveShown = false;

	// 인벤토리 맵을 읽어 슬롯에 설정
	for (const TPair<UItemBase*, FItemSlot>& ItemInMap : InvRef->InventoryMap)
	{
		int32 ItemQuantity = ItemInMap.Value.Quantity;
		int32 ItemMaxQuantity = ItemInMap.Value.MaxQuantity;

		CustomSetItemByQuantity(ItemQuantity, ItemMaxQuantity, ItemInMap.Key,
				[&]() -> void
			{
				SetItemSlotByIndex(UniformGrid_Weapon, ItemInMap.Key, ItemQuantity > ItemMaxQuantity ? ItemMaxQuantity : ItemQuantity, WeaponSlotIndex++);

				// 만약 이 아이템이 장착 중인 아이템이라면
				if (!bEquippedWeaponHaveShown && IsValid(EquippedWeapon) && ItemInMap.Key == EquippedWeapon)
				{
					bEquippedWeaponHaveShown = true;

					// 장착 텍스트 보여주기
					if (UWidget* ChildWidget = UniformGrid_Weapon->GetChildAt(WeaponSlotIndex - 1))
					{
						if (UInventorySlot* SlotWidget = Cast<UInventorySlot>(ChildWidget))
						{
							SlotWidget->ShowEquipText(true);
							EquippedWeaponSlotRef = SlotWidget;
						}
					}
				}
			},
				[&]() -> void
			{
				SetItemSlotByIndex(UniformGrid_Armor, ItemInMap.Key, ItemQuantity > ItemMaxQuantity ? ItemMaxQuantity : ItemQuantity, ArmorSlotIndex++);

				// 만약 이 아이템이 장착 중인 아이템이라면
				if (!bEquippedArmorHaveShown && IsValid(EquippedArmor) && ItemInMap.Key == EquippedArmor)
				{
					bEquippedArmorHaveShown = true;

					// 장착 텍스트 보여주기
					if (UWidget* ChildWidget = UniformGrid_Armor->GetChildAt(ArmorSlotIndex - 1))
					{
						if (UInventorySlot* SlotWidget = Cast<UInventorySlot>(ChildWidget))
						{
							SlotWidget->ShowEquipText(true);
							EquippedArmorSlotRef = SlotWidget;
						}
					}
				}
			},
				[&]() -> void
			{
				SetItemSlotByIndex(UniformGrid_Misc, ItemInMap.Key, ItemQuantity > ItemMaxQuantity ? ItemMaxQuantity : ItemQuantity, MiscSlotIndex++);
			});

		// 0개 보유 중인(즉, 있었는데 없어진) 아이템에 관한 설정
		if (ItemInMap.Value.Quantity == 0)
		{
			SetItemByType(ItemInMap.Key);
		}
	}

	//ForceReloadSkillSlots();

	// 스킬 업데이트
	if (IsValid(PlayerCharacterRef))
	{
		auto Skills = PlayerCharacterRef->GetEquippedSkills();

		UpdateSkillSlot(Skills.Contains(EEnemyAbilityID::AbilityQ) ? Skills[EEnemyAbilityID::AbilityQ] : nullptr, EEnemyAbilityID::AbilityQ);
		UpdateSkillSlot(Skills.Contains(EEnemyAbilityID::AbilityE) ? Skills[EEnemyAbilityID::AbilityE] : nullptr, EEnemyAbilityID::AbilityE);
		UpdateSkillSlot(Skills.Contains(EEnemyAbilityID::AbilityF) ? Skills[EEnemyAbilityID::AbilityF] : nullptr, EEnemyAbilityID::AbilityF);
		UpdateSkillSlot(Skills.Contains(EEnemyAbilityID::AbilityRM) ? Skills[EEnemyAbilityID::AbilityRM] : nullptr, EEnemyAbilityID::AbilityRM);

		PlayerCharacterRef->OnSkillEquipmentChanged.AddUniqueDynamic(this, &UPlayerInventoryWidget::UpdateSkillSlot);
	}
}



void UPlayerInventoryWidget::LoadDefaultSkillSlots()
{
	SetupDefaultSkillSlot(SkillSlot_Q, EEnemyAbilityID::AbilityQ);
	SetupDefaultSkillSlot(SkillSlot_E, EEnemyAbilityID::AbilityE);
	SetupDefaultSkillSlot(SkillSlot_F, EEnemyAbilityID::AbilityF);
	SetupDefaultSkillSlot(SkillSlot_RM, EEnemyAbilityID::AbilityRM);

	SkillSlot_Q->UpdateSkillKeyText(TEXT("Q"));
	SkillSlot_E->UpdateSkillKeyText(TEXT("E"));
	SkillSlot_F->UpdateSkillKeyText(TEXT("F"));
	SkillSlot_RM->UpdateSkillKeyText(TEXT("RMB"));
}



void UPlayerInventoryWidget::SetupDefaultSkillSlot(UInventorySkillSlotWidget* SkillSlotWidget, EEnemyAbilityID AbilityID)
{
	SkillSlotWidget->AbilityID = AbilityID;

	SkillSlotWidget->OnMouseEnteredToSlot.AddUniqueDynamic(this, &UPlayerInventoryWidget::ShowSkillDetailsWidget);
	SkillSlotWidget->OnMouseLeaveFromSlot.AddUniqueDynamic(this, &UPlayerInventoryWidget::HideSkillDetailsWidget);
}



void UPlayerInventoryWidget::UpdateSkillSlot(UItem_CharacterSkillBase* SkillItem, EEnemyAbilityID AbilityID)
{
	UInventorySkillSlotWidget* SelectedSlot = nullptr;

	switch (AbilityID)
	{
	case EEnemyAbilityID::AbilityQ:
		SelectedSlot = SkillSlot_Q;
		break;
	case EEnemyAbilityID::AbilityE:
		SelectedSlot = SkillSlot_E;
		break;
	case EEnemyAbilityID::AbilityF:
		SelectedSlot = SkillSlot_F;
		break;
	case EEnemyAbilityID::AbilityRM:
		SelectedSlot = SkillSlot_RM;
		break;
	default:
		return;
	}

	SelectedSlot->UpdateSlot(SkillItem);
}



void UPlayerInventoryWidget::ShowItemDetailsWidget(UItemBase* Item)
{
	if (!IsValid(Item)) return;

	ItemDetailsWidget->UpdateItem(Item);

	ItemDetailsWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}



void UPlayerInventoryWidget::HideItemDetailsWidget(UItemBase* Item)
{
	ItemDetailsWidget->SetVisibility(ESlateVisibility::Collapsed);
}



void UPlayerInventoryWidget::ShowSkillDetailsWidget(UItem_CharacterSkillBase* SkillItem)
{
	if (!IsValid(SkillItem)) return;

	SkillDetailsWidget->UpdateSkillDetails(SkillItem);

	SkillDetailsWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}



void UPlayerInventoryWidget::HideSkillDetailsWidget(UItem_CharacterSkillBase* SkillItem)
{
	SkillDetailsWidget->SetVisibility(ESlateVisibility::Collapsed);
}



void UPlayerInventoryWidget::UpdateStatTexts()
{
	if (ACharacterBase* PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn<ACharacterBase>())
	{
#define LOCTEXT_NAMESPACE "InventoryStatTextNamespace"

		HpText->SetText(FText::Format(LOCTEXT("InvHpText", "HP: {0}"), FText::AsNumber(PlayerCharacterRef->GetHealth())));
		StrText->SetText(FText::Format(LOCTEXT("InvStrengthText", "STR: {0}"), FText::AsNumber(PlayerCharacterRef->GetStrength())));
		DefText->SetText(FText::Format(LOCTEXT("InvDefenseText", "DEF: {0}"), FText::AsNumber(PlayerCharacterRef->GetDefense())));

#undef LOCTEXT_NAMESPACE
	}
}



void UPlayerInventoryWidget::UpdateHealthText(float NewHealth)
{
#define LOCTEXT_NAMESPACE "InventoryHealthTextNamespace"
	HpText->SetText(FText::Format(LOCTEXT("InvHpText", "HP: {0}"), FText::AsNumber(NewHealth)));
#undef LOCTEXT_NAMESPACE
}



void UPlayerInventoryWidget::UpdateEquipButtonInfo()
{
	UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();

	// 무기 또는 방어구인 경우 장착 버튼 보이게 하기
	if (IsValid(SelectedSlotRef->ItemHas) && (SelectedSlotRef->ItemHas->ItemType == AssetManager.ItemType_Weapon || SelectedSlotRef->ItemHas->ItemType == AssetManager.ItemType_Armor))
	{
		Button_EquipItem->SetVisibility(ESlateVisibility::Visible);
		Button_EquipItem->SetIsEnabled(true);


		// 무기라면 언제든 보임
		if (SelectedSlotRef->ItemHas->ItemType == AssetManager.ItemType_Weapon)
		{
			EquipItemButtonText->SetText(FText::FromString(SelectedSlotRef->IsEquipTextVisible() ? TEXT("장착 해제") : TEXT("장착")));
		}
		// 방어구라면, 마을에서만 장착 가능
		else if (SelectedSlotRef->ItemHas->ItemType == AssetManager.ItemType_Armor)
		{
			UGISS_ShuffleLevel* ShuffleLvSS = GetGameInstance()->GetSubsystem<UGISS_ShuffleLevel>();
			
			if (ShuffleLvSS->CurrentLevelCount <= 0)
			{
				EquipItemButtonText->SetText(FText::FromString(SelectedSlotRef->IsEquipTextVisible() ? TEXT("장착 해제") : TEXT("장착")));
			}
			else
			{
				Button_EquipItem->SetIsEnabled(false);

				// 마을에서만 장착 가능하므로 장착 불가라고 띄우기
				EquipItemButtonText->SetText(FText::FromString(TEXT("장착 불가")));
			}
		}
	}
	// 무기 또는 방어구가 아니라면 장착 버튼 감추기
	else
	{
		Button_EquipItem->SetVisibility(ESlateVisibility::Collapsed);
		Button_EquipItem->SetIsEnabled(true);
	}
}



void UPlayerInventoryWidget::OnEquipButtonClicked()
{
	ACharacterBase* PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn<ACharacterBase>();

	if (!IsValid(SelectedSlotRef) || !IsValid(SelectedSlotRef->ItemHas) || !IsValid(PlayerCharacterRef)) return;

	UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();

	// 무기일 경우
	if (SelectedSlotRef->ItemHas->ItemType == AssetManager.ItemType_Weapon)
	{
		if (IsValid(EquippedWeaponSlotRef)) EquippedWeaponSlotRef->ShowEquipText(false);

		// 장착한 아이템과 선택한 아이템이 다른 경우 장착
		if (EquippedWeaponSlotRef != SelectedSlotRef)
		{
			PlayerCharacterRef->EquipWeapon(Cast<UItem_WeaponBase>(SelectedSlotRef->ItemHas));
			EquippedWeaponSlotRef = SelectedSlotRef;
			EquippedWeaponSlotRef->ShowEquipText(true);
		}
		// 이미 장착한 아이템을 선택한 경우
		else
		{
			// 장착 해제
			PlayerCharacterRef->UnEquipWeapon();
			EquippedWeaponSlotRef = nullptr;
		}
	}
	// 방어구일 경우
	else if (SelectedSlotRef->ItemHas->ItemType == AssetManager.ItemType_Armor)
	{
		UGISS_ShuffleLevel* ShuffleLvSS = GetGameInstance()->GetSubsystem<UGISS_ShuffleLevel>();

		// 마을이 아닐 경우 return (마을에서만 장착 가능)
		if (ShuffleLvSS->CurrentLevelCount > 0) return;

		if (IsValid(EquippedArmorSlotRef)) EquippedArmorSlotRef->ShowEquipText(false);

		// 장착한 아이템과 선택한 아이템이 다른 경우 장착
		if (EquippedArmorSlotRef != SelectedSlotRef)
		{
			PlayerCharacterRef->EquipArmor(Cast<UItem_ArmorBase>(SelectedSlotRef->ItemHas));
			EquippedArmorSlotRef = SelectedSlotRef;
			EquippedArmorSlotRef->ShowEquipText(true);
			EquippedArmorItemImage->SetBrush(SelectedSlotRef->ItemHas->ItemIcon);
			EquippedArmorItemImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		// 이미 장착한 아이템을 선택한 경우
		else
		{
			// 장착 해제
			PlayerCharacterRef->UnEquipArmor();
			EquippedArmorSlotRef = nullptr;

			FSlateBrush NewBrush;

			NewBrush = FSlateBrush();
			NewBrush.SetResourceObject(nullptr);
			EquippedArmorItemImage->SetBrush(NewBrush);
			EquippedArmorItemImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	UpdateStatTexts();
	UpdateEquipButtonInfo();
}



void UPlayerInventoryWidget::OnInventoryUpdated(UItemBase* Item, int32 NewItemCount, int32 DeltaQuantity)
{
	UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();

	int32 ItemQuantity = DeltaQuantity;
	int32 ItemMaxQuantity = Item->MaxCount;

	if (InvSS->InventoryMap.Contains(Item))
	{
		CustomSetItemByQuantity(ItemQuantity, ItemMaxQuantity, Item,
				[&]() -> void
			{
				UpdateSlotOnInventoryChanged(UniformGrid_Weapon, WeaponSlotIndex, Item, ItemQuantity, ItemMaxQuantity);
			},
				[&]() -> void
			{
				// 23.04.18: 아이템을 먹을 때마다 n개씩 추가 획득되는 것처럼 보이는 현상이 있었음
				//            원인은 ItemQuantity를 NewItemCount로 받고 있었기 때문이었고, DeltaQuantity로 변경하여 해결했음
				UpdateSlotOnInventoryChanged(UniformGrid_Armor, ArmorSlotIndex, Item, ItemQuantity, ItemMaxQuantity);
			},
				[&]() -> void
			{
				UpdateSlotOnInventoryChanged(UniformGrid_Misc, MiscSlotIndex, Item, ItemQuantity, ItemMaxQuantity);
			});
	}
	// 새로 획득한 아이템인 경우, 새로 아이템을 설정
	else
	{
		SetItemByType(Item, ItemQuantity);
	}
}



void UPlayerInventoryWidget::SelectSlot(UInventorySlot* SelectedInvSlot)
{
	if (IsValid(SelectedSlotRef)) SelectedSlotRef->ShowHighlight(false);

	SelectedSlotRef = SelectedInvSlot;

	if (!IsValid(SelectedSlotRef)) return;

	SelectedSlotRef->ShowHighlight(true);



	UpdateEquipButtonInfo();
}



void UPlayerInventoryWidget::SetItemSlotByIndex(UUniformGridPanel* UniformGrid, UItemBase* ItemToSet, int32 NewItemCount, int32 Index)
{
	if (UWidget* ChildWidget = UniformGrid->GetChildAt(Index))
	{
		if (UInventorySlot* SlotWidget = Cast<UInventorySlot>(ChildWidget))
		{
			SlotWidget->UpdateItemSlot(ItemToSet, NewItemCount);
		}
	}
}



void UPlayerInventoryWidget::CustomSetItemByQuantity(int32& ItemQuantity, int32 ItemMaxQuantity, UItemBase* Item, TFunction<void()> WeaponFunc, TFunction<void()> ArmorFunc, TFunction<void()> MiscFunc)
{
	UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();

	// 보유량이 최대치를 넘는 경우 옆 칸으로 이동하여 설정
	while (ItemQuantity > 0)
	{
		// 무기인 경우
		if (Item->ItemType == AssetManager.ItemType_Weapon)
		{
			WeaponFunc();
		}
		// 방어구인 경우
		else if (Item->ItemType == AssetManager.ItemType_Armor)
		{
			ArmorFunc();
		}
		// 재료인 경우
		else if (Item->ItemType == AssetManager.ItemType_Material)
		{
			MiscFunc();
		}

		ItemQuantity -= ItemMaxQuantity;
	}
}



void UPlayerInventoryWidget::SetItemByType(UItemBase* Item, int32 ItemQuantity)
{
	UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();

	if (Item->ItemType == AssetManager.ItemType_Weapon)
	{
		SetItemSlotByIndex(UniformGrid_Weapon, Item, ItemQuantity, WeaponSlotIndex++);
	}
	else if (Item->ItemType == AssetManager.ItemType_Armor)
	{
		SetItemSlotByIndex(UniformGrid_Armor, Item, ItemQuantity, ArmorSlotIndex++);
	}
	else if (Item->ItemType == AssetManager.ItemType_Material)
	{
		SetItemSlotByIndex(UniformGrid_Misc, Item, ItemQuantity, MiscSlotIndex++);
	}
}



void UPlayerInventoryWidget::UpdateSlotOnInventoryChanged(UUniformGridPanel* UniformGridWidget, int32& SlotIndex, class UItemBase* Item, int32& ItemQuantity, int32 ItemMaxQuantity)
{
	for (const auto& WidgetSlot : UniformGridWidget->GetAllChildren())
	{
		UInventorySlot* InvSlot = Cast<UInventorySlot>(WidgetSlot);

		if (IsValid(InvSlot) && (InvSlot->ItemHas == Item || !IsValid(InvSlot->ItemHas)))
		{
			// 만약 해당 슬롯이 이미 꽉 찬 경우 다음 루프로 진행
			if (InvSlot->ItemCount >= ItemMaxQuantity) continue;
			// 만약 현재 슬롯 아이템 수량을 채웠을 때 최대량을 초과하지 않는 경우 (예: 2개 <= 99개 - 97개)
			else if (ItemQuantity <= ItemMaxQuantity - InvSlot->ItemCount)
			{
				InvSlot->UpdateItemSlot(Item, ItemQuantity + InvSlot->ItemCount);

				++SlotIndex;
				return;
			}
			// 채웠을 때 초과하는 경우 남은 양을 채우고 다음 루프로 진행
			else
			{
				ItemQuantity -= ItemMaxQuantity - InvSlot->ItemCount;
				InvSlot->UpdateItemSlot(Item, ItemMaxQuantity);

				++SlotIndex;
				continue;
			}
		}
	}
}



void UPlayerInventoryWidget::UpdateMoneyTextOnMoneyChanged(float NewMoney)
{
	NeedInfo_Money->UpdateNeedQuantity(FMath::RoundToInt(NewMoney));
}
