
#include "LevelEvents/LvEvent_RandomReinforceStats.h"

#include "CharacterBase.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Player/PlayerCharacterBase.h"
#include "Widget/InteractableNPCWidgetBase.h"
#include "Widget/RandomReinforceStatsWidget.h"

#include "Kismet/KismetMathLibrary.h"



ALvEvent_RandomReinforceStats::ALvEvent_RandomReinforceStats() : Super(),
	ReinforceTable(nullptr),
	bWasAssigned(false)
{

}



void ALvEvent_RandomReinforceStats::OnConfirmButtonClicked()
{
	TArray<FLvEventRandomReinforceStatTableRow*> OutRows;
	ReinforceTable->GetAllRows<FLvEventRandomReinforceStatTableRow>(TEXT(""), OutRows);

	float TotalWeight = 0.0f;

	for (const auto& Row : OutRows)
	{
		TotalWeight += Row->Weight;
	}

	if (TotalWeight <= 0.0f) return;

	float Pivot = UKismetMathLibrary::RandomFloatInRange(0.0f, TotalWeight);

	for (const auto& Row : OutRows)
	{
		if (Pivot < Row->Weight)
		{
			AssignEffect(Row->ReinforceData.ReinforceEffect, Row->ReinforceData.ReinforceAmount, Row->ReinforceStatType);
			return;
		}
		else
		{
			Pivot -= Row->Weight;
		}
	}
}



void ALvEvent_RandomReinforceStats::AssignEffect(const TSubclassOf<UGameplayEffect>& ReinforceEffect, float ReinforceAmount, ERandomReinforceStatType AssignedStatType)
{
	UGISS_TemporaryPlayerData* TempDataSS = GetGameInstance()->GetSubsystem<UGISS_TemporaryPlayerData>();

	TempDataSS->AssignReinforceEffect(ReinforceEffect, ReinforceAmount, true);

	bWasAssigned = true;

	DisableCollision();

	OnAssignCompleted(AssignedStatType);
}



void ALvEvent_RandomReinforceStats::OnInteractUIShown()
{
	if (!IsValid(WidgetToShow_Instance)) return;

	if (URandomReinforceStatsWidget* ConfirmWidget = Cast<URandomReinforceStatsWidget>(WidgetToShow_Instance))
	{
		ConfirmWidget->OnConfirmSuccessed.AddUniqueDynamic(this, &ALvEvent_RandomReinforceStats::OnConfirmButtonClicked);
	}
}



void ALvEvent_RandomReinforceStats::OnInteractUIBeforeHidden()
{
	if (!IsValid(WidgetToShow_Instance)) return;

	if (URandomReinforceStatsWidget* ConfirmWidget = Cast<URandomReinforceStatsWidget>(WidgetToShow_Instance))
	{
		ConfirmWidget->OnConfirmSuccessed.RemoveDynamic(this, &ALvEvent_RandomReinforceStats::OnConfirmButtonClicked);
	}
}



bool ALvEvent_RandomReinforceStats::CanInteract_Implementation()
{
	return !bWasAssigned;
}
