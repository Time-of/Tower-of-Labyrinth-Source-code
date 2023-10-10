
#include "Player/PlayerCharacterBase.h"

#include "GameAssetManager.h"
#include "PlayerControllerHandler.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Abilities/GameplayAbilityBase.h"
#include "GameInstance/GISS_PlayerInventory.h"
#include "GameInstance/GISS_TemporaryPlayerData.h"
#include "Item/Item_ArmorBase.h"
#include "Item/Item_CharacterSkillBase.h"
#include "Item/Item_WeaponBase.h"
#include "Item/ItemBase.h"
#include "Player/Abilities/Attributes/PlayerAttributeSet.h"
#include "Widget/PlayerInventoryWidget.h"

#include <GraduateProject/GraduateProject.h>

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameFramework/CharacterMovementComponent.h"



APlayerCharacterBase::APlayerCharacterBase() : Super(),
	bIsASCInputBound(false),
	DefaultAttribute(nullptr),
	EquippedSkills()
{
	AttributeSetBase = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
}



void APlayerCharacterBase::PostInitProperties()
{
	Super::PostInitProperties();

	if (!IsValid(AttributeSetBase) || AttributeSetBase->IsA(UAttributeSetBase::StaticClass()))
	{
		AttributeSetBase = NewObject<UPlayerAttributeSet>(this, (TEXT("PlayerAttributeSetComp")));
	}
}



void APlayerCharacterBase::OnSkillManaChanged_Implementation(float DeltaSkillMana, const FGameplayTagContainer& SourceTags)
{

}



void APlayerCharacterBase::OnMaxSkillManaChanged_Implementation(float DeltaMaxSkillMana, const FGameplayTagContainer& SourceTags)
{

}



float APlayerCharacterBase::GetSkillMana() const
{
	if (const UPlayerAttributeSet* PlayerAttributeSet = AbilitySystemComponent->GetSet<UPlayerAttributeSet>())
	{
		return PlayerAttributeSet->GetSkillMana();
	}

	return -1.0f;
}



float APlayerCharacterBase::GetMaxSkillMana() const
{
	if (const UPlayerAttributeSet* PlayerAttributeSet = AbilitySystemComponent->GetSet<UPlayerAttributeSet>())
	{
		return PlayerAttributeSet->GetMaxSkillMana();
	}

	return -1.0f;
}



void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();

	InvSS->OnInventoryChangedDelegate.AddUniqueDynamic(this, &APlayerCharacterBase::ObserveEquippedItemsQuantity);

	LoadTemporaryData();

	//AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(TEXT("State")),
	//	EGameplayTagEventType::AnyCountChange).
	//	AddUObject(this, &APlayerCharacterBase::OnBuffTagChange);
}



void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindAbilitySystemInputComponent(PlayerInputComponent);
}



void APlayerCharacterBase::BindAbilitySystemInputComponent(UInputComponent* PlayerInputComponent)
{
	if (!bIsASCInputBound && IsValid(PlayerInputComponent) && IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent,
			FGameplayAbilityInputBinds(TEXT("Confirm"), TEXT("Cancel"),
				FTopLevelAssetPath(GetPathNameSafe(UClass::TryFindTypeSlow<UEnum>(TEXT("/Script/GraduateProject.EEnemyAbilityID")))),
				static_cast<int32>(EEnemyAbilityID::Confirm), static_cast<int32>(EEnemyAbilityID::Cancel)));
	}
}



void APlayerCharacterBase::ObserveEquippedItemsQuantity(UItemBase* Item, int32 NewQuantity, int32 DeltaQuantity)
{
	if (Item == EquippedWeapon && NewQuantity <= 0)
	{
		UnEquipWeapon();
	}

	if (Item == EquippedArmor && NewQuantity <= 0)
	{
		UnEquipArmor();
	}
}



void APlayerCharacterBase::InitializeDefaultAttribute()
{
	if (!IsValid(DefaultAttribute))
	{
		UE_LOG(LogTemp, Fatal, TEXT("%s 오브젝트의 %s(): DefaultAttribute 값 유효하지 않음!"), *GetName(), *FString(__FUNCTION__));
		return;
	}

	if (bDefaultAttributeInitialized)
	{
		GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttribute,
		GetCharacterLevel(), EffectContext);

	if (EffectHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectHandle.Data.Get(),
			AbilitySystemComponent);
	}



	const UAttributeSetBase* AttributeSet = AbilitySystemComponent->GetSet<UAttributeSetBase>();

	AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetInternalMaxGuardAttribute(),
		EGameplayModOp::Override, GetGuard());

	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();

	bDefaultAttributeInitialized = true;
}



void APlayerCharacterBase::EquipSkillItem(UItem_CharacterSkillBase* SkillItemToEquip, EEnemyAbilityID AbilityID)
{
	if (!IsValid(SkillItemToEquip)) return;

	UnEquipSkillItem(AbilityID);

	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(SkillItemToEquip->GrantedAbilityClass,
		SkillItemToEquip->GrantedAbilityLevel,
		static_cast<int32>(AbilityID), this));

	EquippedSkills.Add(AbilityID, SkillItemToEquip);

	OnSkillEquipmentChanged.Broadcast(SkillItemToEquip, AbilityID);

	UE_LOG(LogTemp, Warning, TEXT("스킬 장착: %s, %d"), IsValid(SkillItemToEquip) ? *SkillItemToEquip->GetName() : *FString(TEXT("NONE")),
		static_cast<int32>(AbilityID));
}



void APlayerCharacterBase::UnEquipSkillItem(EEnemyAbilityID AbilityID)
{
	if (EquippedSkills.Contains(AbilityID))
	{
		AbilitySystemComponent->ClearAbility(AbilitySystemComponent->FindAbilitySpecFromClass(EquippedSkills[AbilityID]->GrantedAbilityClass)->Handle);
		
		EquippedSkills.Remove(AbilityID);

		OnSkillEquipmentChanged.Broadcast(nullptr, AbilityID);

		UE_LOG(LogTemp, Warning, TEXT("스킬 장착 해제: %d"),
			static_cast<int32>(AbilityID));
	}
}



void APlayerCharacterBase::EquipWeapon(UItem_WeaponBase* WeaponToEquip)
{
	ACharacterBase::EquipWeapon(WeaponToEquip);

	// 만약 무기에 장착 시 부여할 스킬이 존재한다면 스킬 장착
	if (IsValid(EquippedWeapon) && IsValid(EquippedWeapon->GrantedSkillOnEquip))
	{
		EquipSkillItem(EquippedWeapon->GrantedSkillOnEquip, EEnemyAbilityID::AbilityRM);

		// 만약 즉시 사용 옵션이 켜져있다면 즉시 사용 시도
		if (EquippedWeapon->bUseSkillImmediately)
		{
			AbilitySystemComponent->TryActivateAbilityByClass(EquippedWeapon->GrantedSkillOnEquip->GrantedAbilityClass);
		}
	}
}



void APlayerCharacterBase::UnEquipWeapon()
{
	// 만약 무기에 장착 시 부여할 스킬이 존재했다면 스킬 제거
	if (IsValid(EquippedWeapon) && IsValid(EquippedWeapon->GrantedSkillOnEquip))
	{
		UnEquipSkillItem(EEnemyAbilityID::AbilityRM);
	}

	ACharacterBase::UnEquipWeapon();
}



void APlayerCharacterBase::DieAction()
{
	OnPlayerDied.Broadcast(this);

	Super::DieAction();
}



void APlayerCharacterBase::SaveTemporaryData()
{
	if (UGISS_TemporaryPlayerData* TempDataSS = GetGameInstance()->GetSubsystem<UGISS_TemporaryPlayerData>())
	{
		TempDataSS->CurrentHealth = GetHealth();
		//TempDataSS->CurrentMaxHealth = GetMaxHealth();
		TempDataSS->CurrentSkillMana = GetSkillMana();
		TempDataSS->EquippedWeapon = EquippedWeapon;
		TempDataSS->EquippedArmor = EquippedArmor;
		TempDataSS->EquippedSkills = EquippedSkills;
	}
}



void APlayerCharacterBase::LoadTemporaryData()
{
	UGISS_TemporaryPlayerData* TempDataSS = GetGameInstance()->GetSubsystem<UGISS_TemporaryPlayerData>();
	UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();

	// CurrentHealth 값 역시 유효한 경우에만 수행
	if (IsValid(TempDataSS) && TempDataSS->CurrentHealth > 0.0f)
	{
		//AttributeSetBase->SetMaxHealth(TempDataSS->CurrentMaxHealth);
		
		// 임시 데이터에 저장되었던 플레이어 강화 이펙트를 적용
		for (const auto& ReinforceEffectInfo : TempDataSS->ReinforceEffectsArray)
		{
			TempDataSS->AssignReinforceEffect(ReinforceEffectInfo.ReinforceEffect, ReinforceEffectInfo.ReinforceAmount, false);
		}

		AttributeSetBase->SetHealth(TempDataSS->CurrentHealth);
		AbilitySystemComponent->ApplyModToAttribute(AbilitySystemComponent->GetSet<UPlayerAttributeSet>()->GetSkillManaAttribute(),
			EGameplayModOp::Override, TempDataSS->CurrentSkillMana);

		UE_LOG(LogTemp, Warning, TEXT("저장되었던 스킬 게이지: %d"), TempDataSS->CurrentSkillMana);

		EquippedSkills.Reset();
		
		for (const auto& Skill : TempDataSS->EquippedSkills)
		{
			EquipSkillItem(Skill.Value, Skill.Key);
		}

		EquipWeapon(TempDataSS->EquippedWeapon);
		EquipArmor(TempDataSS->EquippedArmor);

		UE_LOG(LoadPlayerDataLog, Warning, TEXT("PlayerCharacterBase: 임시 데이터 적용 완료, 무기: %s"), IsValid(TempDataSS->EquippedWeapon) ? *EquippedWeapon->GetName() : *FString(TEXT("없음")));
	}
	else
	{
		UE_LOG(LoadPlayerDataLog, Warning, TEXT("PlayerCharacterBase: 임시 데이터를 불러오려는데 저장된 값이 없거나 유효하지 않음"));
	}

	InvSS->ApplyAllTrainingEffects();
}



bool APlayerCharacterBase::HasSkillItem(UItem_CharacterSkillBase* SkillItem) const
{
	for (const auto& SkillInfo : EquippedSkills)
	{
		if (SkillItem == SkillInfo.Value) return true;
	}

	return false;
}



TMap<EEnemyAbilityID, class UItem_CharacterSkillBase*> APlayerCharacterBase::GetEquippedSkills() const
{
	return EquippedSkills;
}
