
#include "GameInstance/GISS_PlayerMoney.h"

#include "Item/MoneyPickupBase.h"

#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"



UGISS_PlayerMoney::UGISS_PlayerMoney() : Super(),
	Money(0.0f)
{
	static ConstructorHelpers::FClassFinder<AMoneyPickupBase> MoneyPickupBPClass(TEXT("/Game/SungSoo_Workspace/Blueprints/Pickup/BP_MoneyPickup"));

	if (MoneyPickupBPClass.Succeeded())
	{
		MoneyPickupClass = MoneyPickupBPClass.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> UseMoneySoundObject(TEXT("/Game/Assets_Sound/UI/UseMoneyCue"));

	if (UseMoneySoundObject.Succeeded())
	{
		UseMoneySound = UseMoneySoundObject.Object;
	}
}



void UGISS_PlayerMoney::AddMoney(float Amount)
{
	Money += Amount;
	Money = FMath::RoundToInt(Money);
	OnMoneyChanged.Broadcast(Money);
}



float UGISS_PlayerMoney::GetMoney() const
{
	return Money;
}



void UGISS_PlayerMoney::SetMoney(float NewAmount)
{
	Money = NewAmount;
	OnMoneyChanged.Broadcast(Money);
}



bool UGISS_PlayerMoney::TryUseMoney(float Amount)
{
	if (Money < Amount) return false;

	Money -= Amount;
	OnMoneyChanged.Broadcast(Money);
	UGameplayStatics::SpawnSound2D(GetWorld(), UseMoneySound);

	return true;
}
