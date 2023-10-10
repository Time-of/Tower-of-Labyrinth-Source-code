
#include "GameInstance/GISS_TemporaryPlayerData.h"

#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Item/Item_ArmorBase.h"
#include "Item/Item_WeaponBase.h"
#include "Player/PlayerCharacterBase.h"
#include "SaveGame/SaveGame_PlayerCharacterData.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

#include <GraduateProject/GraduateProject.h>



UGISS_TemporaryPlayerData::UGISS_TemporaryPlayerData() : Super(),
	CurrentHealth(-1.0f),
	CurrentSkillMana(-1.0f),
	EquippedWeapon(nullptr),
	EquippedArmor(nullptr),
	EquippedSkills(),
	ReinforceEffectsArray(),
	PlayerDataSaveName(TEXT("PlayerCharacterDataSave"))
{

}



void UGISS_TemporaryPlayerData::InitializePlayerData()
{
	CurrentHealth = -1.0f;
	//CurrentMaxHealth = -1.0f;
	CurrentSkillMana = -1.0f;
	EquippedWeapon = nullptr;
	EquippedArmor = nullptr;
	EquippedSkills.Reset();
	ReinforceEffectsArray.Reset(6);
}



void UGISS_TemporaryPlayerData::SavePlayerData(int32 SlotIndex)
{
	if (APlayerCharacterBase* PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacterBase>())
	{
		PlayerCharacterRef->SaveTemporaryData();
	}
	else
	{
		UE_LOG(LoadPlayerDataLog, Warning, TEXT("저장 실패: 플레이어 데이터 저장하려는데 플레이어 캐릭터를 찾을 수 없음"));
		return;
	}

	USaveGame_PlayerCharacterData* CreatedPlayerCharacterDataSaveGame = Cast<USaveGame_PlayerCharacterData>(UGameplayStatics::CreateSaveGameObject(USaveGame_PlayerCharacterData::StaticClass()));

	CreatedPlayerCharacterDataSaveGame->CurrentHealth = CurrentHealth;
	CreatedPlayerCharacterDataSaveGame->CurrentSkillMana = CurrentSkillMana;
	CreatedPlayerCharacterDataSaveGame->EquippedWeapon = EquippedWeapon;
	CreatedPlayerCharacterDataSaveGame->EquippedArmor = EquippedArmor;

	UGameplayStatics::SaveGameToSlot(CreatedPlayerCharacterDataSaveGame, PlayerDataSaveName, SlotIndex);

	UE_LOG(LoadPlayerDataLog, Warning, TEXT("저장: 플레이어 데이터 완료, 무기: %s, 체력: %f"), IsValid(EquippedWeapon) ? *EquippedWeapon->GetName() : *FString(TEXT("없음"), CurrentHealth));
}



void UGISS_TemporaryPlayerData::LoadPlayerData(int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(PlayerDataSaveName, SlotIndex))
	{
		USaveGame_PlayerCharacterData* PlayerCharacterDataSaveGame = Cast<USaveGame_PlayerCharacterData>(UGameplayStatics::LoadGameFromSlot(PlayerDataSaveName, SlotIndex));

		CurrentHealth = PlayerCharacterDataSaveGame->CurrentHealth;
		CurrentSkillMana = PlayerCharacterDataSaveGame->CurrentSkillMana;
		EquippedWeapon = PlayerCharacterDataSaveGame->EquippedWeapon;
		EquippedArmor = PlayerCharacterDataSaveGame->EquippedArmor;

		UE_LOG(LoadPlayerDataLog, Warning, TEXT("불러오기: 플레이어 데이터 완료, 무기: %s, 체력: %f"), IsValid(EquippedWeapon) ? *EquippedWeapon->GetName() : *FString(TEXT("없음"), CurrentHealth));
	}
	else
	{
		UE_LOG(LoadPlayerDataLog, Warning, TEXT("불러오기 실패: 저장된 플레이어 데이터 없음"));
	}
}



void UGISS_TemporaryPlayerData::AssignReinforceEffect(const TSubclassOf<class UGameplayEffect>& ReinforceEffect, float ReinforceAmount, bool bAddToArray)
{
	if (APlayerCharacterBase* PCRef = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacterBase>())
	{
		UAbilitySystemComponent* ASC = PCRef->GetAbilitySystemComponent();

		auto SpecHandle = ASC->MakeOutgoingSpec(ReinforceEffect, 1.0f, ASC->MakeEffectContext());
		
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,
			FGameplayTag::RequestGameplayTag(TEXT("Data.ReinforceStatAmount")),
			ReinforceAmount);

		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		if (bAddToArray)
		{
			ReinforceEffectsArray.Add(FReinforceEffectData(ReinforceEffect, ReinforceAmount));
		}
	}
}
