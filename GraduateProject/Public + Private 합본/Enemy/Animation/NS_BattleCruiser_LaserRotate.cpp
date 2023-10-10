
#include "Enemy/Animation/NS_BattleCruiser_LaserRotate.h"

#include "Enemy/EnemyCharacterBase.h"
#include "Enemy/AI/BBKeys.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"



UNS_BattleCruiser_LaserRotate::UNS_BattleCruiser_LaserRotate() : Super(),
	TotalTime(0.0f), ElapsedTime(0.0f)
{
	//NotifyColor = FColor(120, 120, 200, 255);
}



void UNS_BattleCruiser_LaserRotate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!GWorld->HasBegunPlay()) return;

	TotalTime = TotalDuration;
	ElapsedTime = 0.0f;

	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}



void UNS_BattleCruiser_LaserRotate::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (!GWorld->HasBegunPlay() || !IsValid(MeshComp)) return;

	ElapsedTime += FrameDeltaTime;

	float RotatePercent = ElapsedTime / TotalTime;

	const FRotator TargetRotation = InternalGetTargetRotation(MeshComp);

	MeshComp->GetOwner()->SetActorRotation(UKismetMathLibrary::RLerp(MeshComp->GetOwner()->GetActorRotation(), TargetRotation, RotatePercent, true));

	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}



void UNS_BattleCruiser_LaserRotate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

}



const FRotator UNS_BattleCruiser_LaserRotate::InternalGetTargetRotation(USkeletalMeshComponent* MeshComp) const
{
	if (!IsValid(MeshComp->GetOwner())) return FRotator::ZeroRotator;

	APawn* PawnRef = UGameplayStatics::GetPlayerPawn(MeshComp->GetOwner(), 0);

	if (!IsValid(PawnRef)) return MeshComp->GetOwner()->GetActorRotation();


	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(MeshComp->GetOwner()->GetActorLocation(), PawnRef->GetActorLocation());
	LookAtRot.Pitch = FMath::Max(LookAtRot.Pitch, -60.0f);
	
	return LookAtRot;
}