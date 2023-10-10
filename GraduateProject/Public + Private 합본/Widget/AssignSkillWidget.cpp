
#include "Widget/AssignSkillWidget.h"

#include "Item/Item_CharacterSkillBase.h"
#include "Player/PlayerCharacterBase.h"
#include "Widget/InventorySkillSlotWidget.h"

#include "Components/Button.h"



UAssignSkillWidget::UAssignSkillWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	SkillItemToAssign(nullptr),
	SkillSlot_Q(nullptr), SkillSlot_E(nullptr), SkillSlot_F(nullptr)
{

}



void UAssignSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LoadDefaultSkillSlots();
}



void UAssignSkillWidget::NativeDestruct()
{
	Super::NativeDestruct();

	SkillSlot_Q->OnSkillButtonClicked.RemoveDynamic(this, &UAssignSkillWidget::AssignSkillOnClicked);
	SkillSlot_E->OnSkillButtonClicked.RemoveDynamic(this, &UAssignSkillWidget::AssignSkillOnClicked);
	SkillSlot_F->OnSkillButtonClicked.RemoveDynamic(this, &UAssignSkillWidget::AssignSkillOnClicked);
}



void UAssignSkillWidget::LoadDefaultSkillSlots()
{
	SetupDefaultSkillSlot(SkillSlot_Q, EEnemyAbilityID::AbilityQ);
	SetupDefaultSkillSlot(SkillSlot_E, EEnemyAbilityID::AbilityE);
	SetupDefaultSkillSlot(SkillSlot_F, EEnemyAbilityID::AbilityF);

	SkillSlot_Q->UpdateSkillKeyText(TEXT("Q"));
	SkillSlot_E->UpdateSkillKeyText(TEXT("E"));
	SkillSlot_F->UpdateSkillKeyText(TEXT("F"));

	APlayerCharacterBase* PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacterBase>();

	if (IsValid(PlayerCharacterRef))
	{
		auto Skills = PlayerCharacterRef->GetEquippedSkills();

		UpdateSkillSlot(Skills.Contains(EEnemyAbilityID::AbilityQ) ? Skills[EEnemyAbilityID::AbilityQ] : nullptr, EEnemyAbilityID::AbilityQ);
		UpdateSkillSlot(Skills.Contains(EEnemyAbilityID::AbilityE) ? Skills[EEnemyAbilityID::AbilityE] : nullptr, EEnemyAbilityID::AbilityE);
		UpdateSkillSlot(Skills.Contains(EEnemyAbilityID::AbilityF) ? Skills[EEnemyAbilityID::AbilityF] : nullptr, EEnemyAbilityID::AbilityF);
	}
}



void UAssignSkillWidget::SetupDefaultSkillSlot(UInventorySkillSlotWidget* SkillSlotWidget, EEnemyAbilityID AbilityID)
{
	SkillSlotWidget->AbilityID = AbilityID;
	SkillSlotWidget->OnSkillButtonClicked.AddUniqueDynamic(this, &UAssignSkillWidget::AssignSkillOnClicked);
}



void UAssignSkillWidget::UpdateSkillSlot(UItem_CharacterSkillBase* SkillItem, EEnemyAbilityID AbilityID)
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
	default:
		return;
	}

	SelectedSlot->UpdateSlot(SkillItem);
}



void UAssignSkillWidget::AssignSkillOnClicked(EEnemyAbilityID AbilityID)
{
	if (!IsValid(SkillItemToAssign))
	{
		UE_LOG(LogTemp, Warning, TEXT("스킬 부여: 부여할 스킬이 유효하지 않습니다!!!"));

		return;
	}

	APlayerCharacterBase* PlayerCharacterRef = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacterBase>();

	if (IsValid(PlayerCharacterRef))
	{
		PlayerCharacterRef->EquipSkillItem(SkillItemToAssign, AbilityID);

		UE_LOG(LogTemp, Warning, TEXT("스킬 부여: 스킬 %s를 %d에 부여 성공"), *SkillItemToAssign->GetName(), static_cast<int32>(AbilityID));

		OnAssignSuccessed.Broadcast();

		RemoveThisFromParent();
	}
}
