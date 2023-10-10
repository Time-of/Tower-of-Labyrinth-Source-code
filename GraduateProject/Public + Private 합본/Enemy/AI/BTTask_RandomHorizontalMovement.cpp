
#include "Enemy/AI/BTTask_RandomHorizontalMovement.h"

#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"



UBTTask_RandomHorizontalMovement::UBTTask_RandomHorizontalMovement() : Super(),
	Direction(1.0f),
	BackwardMovementInput(0.0f),
	bAlsoRandomMoveBackward(false)
{
	NodeName = TEXT("좌/우 무작위 방향으로 이동");

	bNotifyTick = true;
}



EBTNodeResult::Type UBTTask_RandomHorizontalMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Direction = UKismetMathLibrary::RandomBool() ? 1.0f : -1.0f;

	BackwardMovementInput = bAlsoRandomMoveBackward ? UKismetMathLibrary::RandomFloatInRange(0.0f, -0.7f) : 0.0f;

	return EBTNodeResult::InProgress;
}



void UBTTask_RandomHorizontalMovement::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), Direction);
	ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), BackwardMovementInput);
}



EBTNodeResult::Type UBTTask_RandomHorizontalMovement::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}
