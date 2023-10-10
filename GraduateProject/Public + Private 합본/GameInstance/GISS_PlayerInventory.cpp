
#include "GameInstance/GISS_PlayerInventory.h"

#include "CharacterBase.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "GameInstance/GISS_PlayerMoney.h"
#include "Item/ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SaveGame_Inventory.h"

#include "GameAssetManager.h"

#include <GraduateProject/GraduateProject.h>



UGISS_PlayerInventory::UGISS_PlayerInventory() : Super(),
	InventorySaveName(TEXT("InventorySaveData"))
{
	TrainingAttributeLevelMap.Add(ETrainingAttributeType::MAXHEALTH, 0.0f);
	TrainingAttributeLevelMap.Add(ETrainingAttributeType::DEFENSE, 0.0f);
	TrainingAttributeLevelMap.Add(ETrainingAttributeType::STRENGTH, 0.0f);

	static ConstructorHelpers::FClassFinder<UGameplayEffect> DefaultTrainingMaxHealthClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Effects/Training_Reinforcement/GE_Reinforce_Training_MaxHealth");
	if (DefaultTrainingMaxHealthClass.Succeeded()) Effect_MaxHealthReinforce = DefaultTrainingMaxHealthClass.Class;

	static ConstructorHelpers::FClassFinder<UGameplayEffect> DefaultTrainingDefenseClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Effects/Training_Reinforcement/GE_Reinforce_Training_Defense");
	if (DefaultTrainingDefenseClass.Succeeded()) Effect_DefenseReinforce = DefaultTrainingDefenseClass.Class;

	static ConstructorHelpers::FClassFinder<UGameplayEffect> DefaultTrainingStrengthClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Effects/Training_Reinforcement/GE_Reinforce_Training_Strength");
	if (DefaultTrainingStrengthClass.Succeeded()) Effect_StrengthReinforce = DefaultTrainingStrengthClass.Class;
}



void UGISS_PlayerInventory::AddItem(const FPrimaryAssetId& PrimaryAssetId, int32 Quantity)
{
	UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();
	UItemBase* LoadedItem = AssetManager.LoadItem(PrimaryAssetId);

	AddItem(LoadedItem, Quantity);
}



void UGISS_PlayerInventory::AddItem(UItemBase* Item, int32 Quantity)
{
	if (!IsValid(Item)) return;

	int32 DeltaQuantity;

	if (InventoryMap.Contains(Item))
	{
		DeltaQuantity = InventoryMap[Item].Quantity;
		InventoryMap[Item].AddQuantity(Quantity);
		DeltaQuantity = InventoryMap[Item].Quantity - DeltaQuantity;
	}
	else
	{
		DeltaQuantity = Quantity;
		InventoryMap.Add(Item, FItemSlot(Item, Quantity));
	}

	OnInventoryChangedDelegate.Broadcast(Item, InventoryMap[Item].Quantity, DeltaQuantity);
}



void UGISS_PlayerInventory::SetItemQuantity(UItemBase* Item, int32 Quantity)
{
	if (!IsValid(Item)) return;

	int32 DeltaQuantity;

	if (InventoryMap.Contains(Item))
	{
		DeltaQuantity = Quantity - InventoryMap[Item].Quantity;
		InventoryMap[Item].Quantity = Quantity;
	}
	else
	{
		DeltaQuantity = Quantity;
		InventoryMap.Add(Item, FItemSlot(Item, Quantity));
	}

	OnInventoryChangedDelegate.Broadcast(Item, InventoryMap[Item].Quantity, DeltaQuantity);
}



int32 UGISS_PlayerInventory::TryGetItemQuantity(UItemBase* Item)
{
	if (!IsValid(Item)) return -1;

	if (InventoryMap.Contains(Item))
	{
		return InventoryMap[Item].Quantity;
	}
	else
	{
		return -1;
	}
}



void UGISS_PlayerInventory::InitializeInventoryData()
{
	InventoryMap.Reset();
	TrainingAttributeLevelMap.Reset();

	if (UGISS_PlayerMoney* MoneySS = GetWorld()->GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>())
	{
		MoneySS->SetMoney(0);

		UE_LOG(LoadPlayerDataLog, Warning, TEXT("돈 초기화 완료: %f"), MoneySS->GetMoney());
	}

	TrainingAttributeLevelMap.Add(ETrainingAttributeType::MAXHEALTH, 0.0f);
	TrainingAttributeLevelMap.Add(ETrainingAttributeType::DEFENSE, 0.0f);
	TrainingAttributeLevelMap.Add(ETrainingAttributeType::STRENGTH, 0.0f);
}



void UGISS_PlayerInventory::SaveInventoryData(int32 SlotIndex)
{
	USaveGame_Inventory* CreatedInvSaveGame = Cast<USaveGame_Inventory>(UGameplayStatics::CreateSaveGameObject(USaveGame_Inventory::StaticClass()));

	CreatedInvSaveGame->InventoryMap = InventoryMap;
	CreatedInvSaveGame->TrainingAttributeLevelMap = TrainingAttributeLevelMap;

	for (const auto& Item : CreatedInvSaveGame->InventoryMap)
	{
		UE_LOG(LoadPlayerDataLog, Warning, TEXT("SaveInventory: %s, 개수: %d"), *Item.Key->GetName(), Item.Value.Quantity);
	}
	
	if (UGISS_PlayerMoney* MoneySS = GetWorld()->GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>())
	{
		CreatedInvSaveGame->Money = MoneySS->GetMoney();

		UE_LOG(LoadPlayerDataLog, Warning, TEXT("저장: 돈 완료: %f, 현재 돈: %f"), CreatedInvSaveGame->Money, MoneySS->GetMoney());
	}

	UGameplayStatics::SaveGameToSlot(CreatedInvSaveGame, InventorySaveName, SlotIndex);

	UE_LOG(LoadPlayerDataLog, Warning, TEXT("저장: 인벤토리 완료"));
}



void UGISS_PlayerInventory::LoadInventoryData(int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(InventorySaveName, SlotIndex))
	{
		USaveGame_Inventory* InvSaveGame = Cast<USaveGame_Inventory>(UGameplayStatics::LoadGameFromSlot(InventorySaveName, SlotIndex));

		InventoryMap.Reset();
		TrainingAttributeLevelMap.Reset();

		InventoryMap = InvSaveGame->InventoryMap;
		TrainingAttributeLevelMap = InvSaveGame->TrainingAttributeLevelMap;

		// 훈련 데이터를 모두 적용하는 부분은 플레이어 캐릭터에서 수행
		//ApplyAllTrainingEffects();

		// 돈 저장 정보를 가져오기
		if (UGISS_PlayerMoney* MoneySS = GetWorld()->GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>())
		{
			MoneySS->SetMoney(InvSaveGame->Money);

			UE_LOG(LoadPlayerDataLog, Warning, TEXT("불러오기: 돈 완료: %f"), InvSaveGame->Money);
		}

		UE_LOG(LoadPlayerDataLog, Warning, TEXT("불러오기: 인벤토리 완료"));
	}
	// 세이브게임이 유효하지 않으면 초기화
	else
	{
		InitializeInventoryData();

		if (UGISS_PlayerMoney* MoneySS = GetWorld()->GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>())
		{
			MoneySS->SetMoney(0);

			UE_LOG(LoadPlayerDataLog, Warning, TEXT("불러오기 실패: 저장된 돈 없음"));
		}

		UE_LOG(LoadPlayerDataLog, Warning, TEXT("불러오기 실패: 저장된 인벤토리 없음"));
	}
}



void UGISS_PlayerInventory::ReloadAllWeaponsArmorsSkills(int32 SlotIndex)
{
	TArray<UItemBase*> ItemsToDelete;

	// 먼저 무기, 방어구, 스킬 아이템을 전부 삭제
	for (const auto& Item : InventoryMap)
	{
		if (Item.Key->ItemType == UGameAssetManager::ItemType_Weapon ||
			Item.Key->ItemType == UGameAssetManager::ItemType_Armor ||
			Item.Key->ItemType == UGameAssetManager::ItemType_CharacterSkill)
		{
			ItemsToDelete.Add(Item.Key);
		}
	}

	for (const auto& Item : ItemsToDelete)
	{
		InventoryMap.Remove(Item);
	}



	if (!UGameplayStatics::DoesSaveGameExist(InventorySaveName, SlotIndex)) return;

	USaveGame_Inventory* InvSaveGame = Cast<USaveGame_Inventory>(UGameplayStatics::LoadGameFromSlot(InventorySaveName, SlotIndex));



	// 세이브게임으로부터 무기, 방어구, 스킬 아이템을 전부 로드
	for (const auto& Item : InvSaveGame->InventoryMap)
	{
		if (Item.Key->ItemType == UGameAssetManager::ItemType_Weapon ||
			Item.Key->ItemType == UGameAssetManager::ItemType_Armor ||
			Item.Key->ItemType == UGameAssetManager::ItemType_CharacterSkill)
		{
			UE_LOG(LoadPlayerDataLog, Warning, TEXT("ReloadAllWeaponArmors: %s, 개수: %d"), *Item.Key->GetName(), Item.Value.Quantity);

			InventoryMap.Add(Item);
		}
	}
}



void UGISS_PlayerInventory::ApplyAllTrainingEffects()
{
	if (ACharacterBase* PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetPawn<ACharacterBase>())
	{
		if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
		{
			for (const auto& TrainingDataPair : TrainingAttributeLevelMap)
			{
				TSubclassOf<UGameplayEffect> EffectToApply;

				switch (TrainingDataPair.Key)
				{
				default:
				case ETrainingAttributeType::NONE:
					continue;

				case ETrainingAttributeType::MAXHEALTH:
					EffectToApply = Effect_MaxHealthReinforce;
					break;

				case ETrainingAttributeType::DEFENSE:
					EffectToApply = Effect_DefenseReinforce;
					break;

				case ETrainingAttributeType::STRENGTH:
					EffectToApply = Effect_StrengthReinforce;
					break;
				}

				FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectToApply,
					TrainingDataPair.Value, ASC->MakeEffectContext());

				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}
