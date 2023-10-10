
#include "Widget/PlayerSkillHudWidget.h"

#include "GameInstance/GISS_PlayerInventory.h"
#include "Item/Item_CharacterSkillBase.h"
#include "Player/PlayerCharacterBase.h"
#include "Widget/InventorySkillSlotWidget.h"



UPlayerSkillHudWidget::UPlayerSkillHudWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	SkillSlot_Q(nullptr),
	SkillSlot_E(nullptr),
	SkillSlot_F(nullptr),
	SkillSlot_RMB(nullptr)
{

}



void UPlayerSkillHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LoadDefaultSkillSlots();
}



void UPlayerSkillHudWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UWorld* WorldRef = GetWorld();
	APlayerController* PCRef = IsValid(WorldRef) ? WorldRef->GetFirstPlayerController() : nullptr;
	APlayerCharacterBase* PlayerCharacterRef = IsValid(PCRef) ? PCRef->GetPawn<APlayerCharacterBase>() : nullptr;

	if (IsValid(PlayerCharacterRef))
	{
		PlayerCharacterRef->OnSkillEquipmentChanged.RemoveDynamic(this, &UPlayerSkillHudWidget::UpdateSkillSlot);
	}
}



void UPlayerSkillHudWidget::LoadDefaultSkillSlots()
{
	SetupDefaultSkillSlot(SkillSlot_Q, EEnemyAbilityID::AbilityQ);
	SetupDefaultSkillSlot(SkillSlot_E, EEnemyAbilityID::AbilityE);
	SetupDefaultSkillSlot(SkillSlot_F, EEnemyAbilityID::AbilityF);
	SetupDefaultSkillSlot(SkillSlot_RMB, EEnemyAbilityID::AbilityRM);

	SkillSlot_Q->UpdateSkillKeyText(TEXT("Q"));
	SkillSlot_E->UpdateSkillKeyText(TEXT("E"));
	SkillSlot_F->UpdateSkillKeyText(TEXT("F"));
	SkillSlot_RMB->UpdateSkillKeyText(TEXT("RMB"));

	APlayerCharacterBase* PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacterBase>();

	if (IsValid(PlayerCharacterRef))
	{
		auto Skills = PlayerCharacterRef->GetEquippedSkills();

		UpdateSkillSlot(Skills.Contains(EEnemyAbilityID::AbilityQ) ? Skills[EEnemyAbilityID::AbilityQ] : nullptr, EEnemyAbilityID::AbilityQ);
		UpdateSkillSlot(Skills.Contains(EEnemyAbilityID::AbilityE) ? Skills[EEnemyAbilityID::AbilityE] : nullptr, EEnemyAbilityID::AbilityE);
		UpdateSkillSlot(Skills.Contains(EEnemyAbilityID::AbilityF) ? Skills[EEnemyAbilityID::AbilityF] : nullptr, EEnemyAbilityID::AbilityF);
		UpdateSkillSlot(Skills.Contains(EEnemyAbilityID::AbilityRM) ? Skills[EEnemyAbilityID::AbilityRM] : nullptr, EEnemyAbilityID::AbilityRM);
	
		PlayerCharacterRef->OnSkillEquipmentChanged.AddUniqueDynamic(this, &UPlayerSkillHudWidget::UpdateSkillSlot);
	}
}



void UPlayerSkillHudWidget::SetupDefaultSkillSlot(UInventorySkillSlotWidget* SkillSlotWidget, EEnemyAbilityID AbilityID)
{
	SkillSlotWidget->AbilityID = AbilityID;
}



void UPlayerSkillHudWidget::UpdateSkillSlot(UItem_CharacterSkillBase* SkillItem, EEnemyAbilityID AbilityID)
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
		SelectedSlot = SkillSlot_RMB;
		break;
	default:
		return;
	}

	SelectedSlot->UpdateSlot(SkillItem);
}

