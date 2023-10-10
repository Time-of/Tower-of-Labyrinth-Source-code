
#include "Widget/InventorySkillSlotWidget.h"

#include "Item/Item_CharacterSkillBase.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"



UInventorySkillSlotWidget::UInventorySkillSlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	AbilityID(EEnemyAbilityID::None),
	SkillButton(nullptr),
	SkillImage(nullptr),
	SkillKeyText(nullptr),
	SkillHas(nullptr)
{

}



void UInventorySkillSlotWidget::UpdateSlot(UItem_CharacterSkillBase* SkillItem)
{
	SkillHas = SkillItem;

	FSlateBrush NewBrush;

	if (!IsValid(SkillHas))
	{
		NewBrush = FSlateBrush();
		NewBrush.SetResourceObject(nullptr);
		NewBrush.TintColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		NewBrush = SkillHas->ItemIcon;
	}

	SkillImage->SetBrush(NewBrush);
}



void UInventorySkillSlotWidget::UpdateSkillKeyText(const FString& TextToUpdate)
{
	if (!IsValid(SkillKeyText)) return;
	SkillKeyText->SetText(FText::FromString(TextToUpdate));
}



void UInventorySkillSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SkillButton->OnClicked.AddDynamic(this, &UInventorySkillSlotWidget::InternalOnSkillButtonClicked);
}



void UInventorySkillSlotWidget::NativeDestruct()
{
	Super::NativeDestruct();

	SkillButton->OnClicked.RemoveDynamic(this, &UInventorySkillSlotWidget::InternalOnSkillButtonClicked);
}



void UInventorySkillSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	OnMouseEnteredToSlot.Broadcast(SkillHas);
}



void UInventorySkillSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	OnMouseLeaveFromSlot.Broadcast(SkillHas);
}



void UInventorySkillSlotWidget::InternalOnSkillButtonClicked()
{
	OnSkillButtonClicked.Broadcast(AbilityID);
}
