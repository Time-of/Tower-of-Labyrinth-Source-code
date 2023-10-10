
#include "Widget/RandomReinforceStatsWidget.h"

#include "Components/Button.h"



URandomReinforceStatsWidget::URandomReinforceStatsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	ConfirmButton(nullptr)
{

}



void URandomReinforceStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ConfirmButton->OnClicked.AddUniqueDynamic(this, &URandomReinforceStatsWidget::InternalOnConfirmButtonClicked);
}



void URandomReinforceStatsWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ConfirmButton->OnClicked.RemoveDynamic(this, &URandomReinforceStatsWidget::InternalOnConfirmButtonClicked);
}



void URandomReinforceStatsWidget::InternalOnConfirmButtonClicked()
{
	OnConfirmSuccessed.Broadcast();
}
