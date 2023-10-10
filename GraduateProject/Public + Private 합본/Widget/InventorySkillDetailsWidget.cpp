
#include "Widget/InventorySkillDetailsWidget.h"

#include "Item/Item_CharacterSkillBase.h"

#include "BlueprintFunctionLibrary/GameplayHelperLibrary.h"
#include "Enemy/Abilities/GameplayAbilityBase.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Player/PlayerCharacterBase.h"
#include "Player/Abilities/Attributes/PlayerAttributeSet.h"
#include "Player/Abilities/GEC_UseSkillMana.h"



UInventorySkillDetailsWidget::UInventorySkillDetailsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
SkillNameText(nullptr), SkillDescriptionText(nullptr), SkillCostText(nullptr)
{

}



void UInventorySkillDetailsWidget::UpdateSkillDetails(UItem_CharacterSkillBase* SkillItem)
{
	if (!IsValid(SkillItem)) return;

	SkillNameText->SetText(SkillItem->ItemName);
	SkillDescriptionText->SetText(SkillItem->ItemDescription);

	if (UGameplayAbilityBase* GABase = Cast<UGameplayAbilityBase>(SkillItem->GrantedAbilityClass->GetDefaultObject()))
	{
		if (GABase->GetCostGameplayEffect()->Modifiers.IsEmpty()) return;

		float SkillCost = GABase->GetCostGameplayEffect()->Modifiers[0].ModifierMagnitude.CustomMagnitude.Coefficient.GetValue();

		float SkillManaUsageRate = 1.0f;

		if (APlayerCharacterBase* PC = UGameplayHelperLibrary::GetPlayerCharacter(this->GetOwningPlayer()))
		{
			if (const UPlayerAttributeSet* AttributeSet = PC->GetAbilitySystemComponent()->GetSet<UPlayerAttributeSet>())
			{
				SkillManaUsageRate = AttributeSet->GetSkillManaUsageRate();
			}
		}

#define LOCTEXT_NAMESPACE "SkillDetailsSkillCostText"
		SkillCostText->SetText(FText::Format(
			LOCTEXT("SkillDetailsSkillCostText", "마나: {0}"),
			FText::AsNumber(FMath::Abs(SkillCost) * SkillManaUsageRate))
		);
#undef LOCTEXT_NAMESPACE

		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("스킬 코스트: %f"), SkillCost));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("스킬 찾을 수 없음")));
	}
}
