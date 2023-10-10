
#include "Enemy/AI/BTS_UpdateDistanceFromTarget.h"

#include "Enemy/AI/BBKeys.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"



UBTS_UpdateDistanceFromTarget::UBTS_UpdateDistanceFromTarget() : Super()
{
	Interval = 0.3f;
	RandomDeviation = 0.0f;
	NodeName = TEXT("타겟과의 거리 업데이트");
}



void UBTS_UpdateDistanceFromTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TArray<AActor*> OutSensedActors;
	AAIController* AIControllerRef = OwnerComp.GetAIOwner();
	AIControllerRef->GetAIPerceptionComponent()->GetCurrentlyPerceivedActors(UAISenseConfig_Sight::StaticClass(), OutSensedActors);


	if (OutSensedActors.IsValidIndex(0) && IsValid(AIControllerRef->GetPawn()))
	{
		AIControllerRef->GetBlackboardComponent()->SetValueAsVector(BBKeys::TargetLocation, OutSensedActors[0]->GetActorLocation());
	
		AIControllerRef->GetBlackboardComponent()->SetValueAsFloat(BBKeys::DistanceFromTarget,
			(AIControllerRef->GetPawn()->GetActorLocation() - OutSensedActors[0]->GetActorLocation()).Size2D());
	}
}
