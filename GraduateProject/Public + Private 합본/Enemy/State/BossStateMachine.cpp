
#include "Enemy/State/BossStateMachine.h"



UBossStateMachine::UBossStateMachine() : Super(),
	CurrentState(nullptr),
	Boss(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;

	
}



void UBossStateMachine::BeginPlay()
{
	Super::BeginPlay();

	if (AEnemy_Boss_Master* OwnerBoss = Cast<AEnemy_Boss_Master>(GetOwner()))
	{
		Boss = OwnerBoss;
	}
}



void UBossStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState != nullptr && CurrentState->bNeedUpdateTick && IsValid(Boss))
	{
		CurrentState->Tick(Boss, DeltaTime);
	}
}



bool UBossStateMachine::ChangeState(const TSharedPtr<BossStateBase>& NewState)
{
	if (CurrentState == NewState || !IsValid(Boss)) return false;

	if (CurrentState != nullptr) CurrentState->Exit(Boss);

	CurrentState = NewState;

	CurrentState->Enter(Boss);

	return true;
}
