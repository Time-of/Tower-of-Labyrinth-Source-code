
#include "Enemy/Animation/NS_MotionWarpTowards.h"

#include "Kismet/KismetMathLibrary.h"



UNS_MotionWarpTowards::UNS_MotionWarpTowards() : UNS_EnemyMotionWarp(),
	SourceFinalLocation(),
	SourceRotation()
{
}



void UNS_MotionWarpTowards::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!GWorld->HasBegunPlay()) return;

	UNS_EnemyMotionWarp::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	SourceFinalLocation = FinalLocation;
	SourceRotation = MeshComp->GetOwner()->GetActorRotation();
}



void UNS_MotionWarpTowards::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (GWorld->HasBegunPlay() && bUseLocalLocation)
	{
		// 이동해야 하는 위치를 계산
		// 목표 지점 = 현재 위치 + (이동해야 하는 벡터의 Yaw 회전한 결과)
		float YawDelta = UKismetMathLibrary::NormalizedDeltaRotator(MeshComp->GetOwner()->GetActorRotation(), SourceRotation).Yaw;

		FinalLocation = LastLocation + (SourceFinalLocation - FMath::Lerp(CurrentLocation, SourceFinalLocation, TargetAlpha)).RotateAngleAxis(YawDelta, FVector::UpVector);
	}

	UNS_EnemyMotionWarp::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}
