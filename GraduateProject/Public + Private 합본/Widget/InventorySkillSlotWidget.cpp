
#include "Widget/InventorySkillSlotWidget.h"

#include "Item/Item_CharacterSkillBase.h"

#include "BlueprintFunctionLibrary/GameplayHelperLibrary.h"
#include "Enemy/Abilities/GameplayAbilityBase.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Player/PlayerCharacterBase.h"
#include "Player/Abilities/Attributes/PlayerAttributeSet.h"
#include "Player/Abilities/GEC_UseSkillMana.h"


UInventorySkillSlotWidget::UInventorySkillSlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	AbilityID(EEnemyAbilityID::None),
	SkillButton(nullptr),
	SkillImage(nullptr),
	SkillKeyText(nullptr),
	SkillHas(nullptr),
	SkillManaUsage(65535.0f)
{

}


void UInventorySkillSlotWidget::UpdateSlot(UItem_CharacterSkillBase* SkillItem, bool bIsHUD)
{
	SkillHas = SkillItem;

	FSlateBrush NewBrush;

	if (!IsValid(SkillHas))
	{
		NewBrush = FSlateBrush();
		NewBrush.SetResourceObject(nullptr);
		NewBrush.TintColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);

		SkillManaUsageText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		NewBrush = SkillHas->ItemIcon;
	}

	SkillImage->SetBrush(NewBrush);

	// HUD가 아닐 때는 연산이 필요 없음
	if (bIsHUD)
	{
		float SkillManaUsageRate = 1.0f;
		float CurrentSkillMana = 0.0f;

		if (APlayerCharacterBase* PC = UGameplayHelperLibrary::GetPlayerCharacter(this->GetOwningPlayer()))
		{
			SkillManaUsageRate = PC->GetSkillManaUsageRate();
			CurrentSkillMana = PC->GetSkillMana();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: 플레이어 캐릭터가 유효하지 않습니다!"), *FString(__FUNCTION__));
		}

		SkillManaUsageText->SetVisibility(IsValid(SkillHas) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		RefreshSkillManaUsage(SkillManaUsageRate);
		RefreshButtonActivationBySkillCost(CurrentSkillMana);
		UpdateSkillManaUsageText();
	}
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


void UInventorySkillSlotWidget::RefreshButtonActivationBySkillCost(float CurrentPlayerSkillMana)
{
	if (!IsValid(SkillHas))
	{
		SkillManaUsage = 65535.0f;
		SkillButton->SetIsEnabled(true);
		return;
	}

	// 오버드라이브 같이, 스킬 마나 사용량을 찾지 못한 케이스에 대한 처리
	SkillButton->SetIsEnabled(SkillManaUsage <= 10000.0f ? CurrentPlayerSkillMana >= SkillManaUsage : true);
}


void UInventorySkillSlotWidget::RefreshSkillManaUsage(float NewSkillManaUsageRate)
{
	if (!IsValid(SkillHas))
	{
		SkillManaUsage = 65535.0f;
		return;
	}


	if (UGameplayAbilityBase* GABase = Cast<UGameplayAbilityBase>(SkillHas->GrantedAbilityClass->GetDefaultObject()))
	{
		if (!IsValid(GABase->GetCostGameplayEffect())
			|| GABase->GetCostGameplayEffect()->Modifiers.IsEmpty()
			|| !GABase->GetCostGameplayEffect()->Modifiers[0].ModifierMagnitude.CustomMagnitude.Coefficient.IsValid())
		{
			SkillManaUsage = 65535.0f;
			return;
		}

		float SkillCost = GABase->GetCostGameplayEffect()->Modifiers[0].ModifierMagnitude.CustomMagnitude.Coefficient.GetValue();

		SkillManaUsage = FMath::Abs(SkillCost) * NewSkillManaUsageRate;
	}
}


void UInventorySkillSlotWidget::UpdateSkillManaUsageText()
{
	if (!IsValid(SkillHas))
	{
		return;
	}

	SkillManaUsageText->SetText(SkillHas->bOverrideCostText ? SkillHas->NewCostTextOnHUD : FText::FromString(FString::FromInt(FMath::RoundToInt(SkillManaUsage))));
}


void UInventorySkillSlotWidget::InternalOnSkillButtonClicked()
{
	OnSkillButtonClicked.Broadcast(AbilityID);
}