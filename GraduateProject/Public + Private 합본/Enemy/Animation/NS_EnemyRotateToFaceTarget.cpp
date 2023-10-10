
#include "Enemy/Animation/NS_EnemyRotateToFaceTarget.h"

#include "Enemy/EnemyCharacterBase.h"
#include "Enemy/AI/BBKeys.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"



UNS_EnemyRotateToFaceTarget::UNS_EnemyRotateToFaceTarget() : Super(),
	TotalTime(1.0f), ElapsedTime(0.0f),
	SourceRotation(FRotator::ZeroRotator),
	RotateStrategy(EEnemyRotateToFaceType::FollowTowards),
	InterpSpeed(3.0f),
	bDontRotateWhenYawDeltaTooLarge(false),
	AbsYawDeltaThreshold(45.0f)
{
	//NotifyColor = FColor(100, 200, 200, 255);
}



void UNS_EnemyRotateToFaceTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!GWorld->HasBegunPlay()) return;

	TotalTime = TotalDuration;
	ElapsedTime = 0.0f;
	SourceRotation = MeshComp->GetOwner()->GetActorRotation();

	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}



void UNS_EnemyRotateToFaceTarget::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (!GWorld->HasBegunPlay() || !IsValid(MeshComp)) return;

	switch (RotateStrategy)
	{
		case EEnemyRotateToFaceType::FollowTowards:
			FollowTowardsStrategy(MeshComp, FrameDeltaTime);
			break;
		case EEnemyRotateToFaceType::Lerp:
			LerpStrategy(MeshComp, FrameDeltaTime);
			break;
		case EEnemyRotateToFaceType::Interp:
			InterpStrategy(MeshComp, FrameDeltaTime);
			break;
	}

	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}



void UNS_EnemyRotateToFaceTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);


}



void UNS_EnemyRotateToFaceTarget::FollowTowardsStrategy(USkeletalMeshComponent* MeshComp, float FrameDeltaTime)
{
	InternalLerpRotation(MeshComp, FrameDeltaTime, MeshComp->GetOwner()->GetActorRotation());
}



void UNS_EnemyRotateToFaceTarget::LerpStrategy(USkeletalMeshComponent* MeshComp, float FrameDeltaTime)
{
	InternalLerpRotation(MeshComp, FrameDeltaTime, SourceRotation);
}



void UNS_EnemyRotateToFaceTarget::InterpStrategy(USkeletalMeshComponent* MeshComp, float FrameDeltaTime)
{
	SetRotation(MeshComp, UKismetMathLibrary::RInterpTo(MeshComp->GetOwner()->GetActorRotation(),
		InternalGetTargetRotation(MeshComp), FrameDeltaTime, InterpSpeed));
}



void UNS_EnemyRotateToFaceTarget::InternalLerpRotation(USkeletalMeshComponent* MeshComp, float FrameDeltaTime, const FRotator& FromRotation)
{
	ElapsedTime += FrameDeltaTime;

	float RotatePercent = ElapsedTime / TotalTime;

	const FRotator TargetRotation = InternalGetTargetRotation(MeshComp);

	SetRotation(MeshComp, UKismetMathLibrary::RLerp(FromRotation, TargetRotation, RotatePercent, true));
}



const FRotator UNS_EnemyRotateToFaceTarget::InternalGetTargetRotation(USkeletalMeshComponent* MeshComp) const
{
	if (!IsValid(MeshComp->GetOwner())) return FRotator::ZeroRotator;

	APawn* PawnRef = UGameplayStatics::GetPlayerPawn(MeshComp->GetOwner(), 0);

	if (!IsValid(PawnRef)) return MeshComp->GetOwner()->GetActorRotation();

	return FRotator(0.0f,
		UKismetMathLibrary::FindLookAtRotation(MeshComp->GetOwner()->GetActorLocation(), PawnRef->GetActorLocation()).Yaw,
		0.0f);
}



void UNS_EnemyRotateToFaceTarget::SetRotation(USkeletalMeshComponent* MeshComp, const FRotator& Rotation)
{
	float YawDelta = UKismetMathLibrary::NormalizedDeltaRotator(Rotation, MeshComp->GetOwner()->GetActorRotation()).Yaw;

	if (!bDontRotateWhenYawDeltaTooLarge)
	{
		MeshComp->GetOwner()->SetActorRotation(Rotation);
	}
	else if (FMath::Abs(YawDelta) < AbsYawDeltaThreshold)
	{
		MeshComp->GetOwner()->SetActorRotation(Rotation);
	}
}
