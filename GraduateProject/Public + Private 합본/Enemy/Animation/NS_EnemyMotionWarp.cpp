
#include "Enemy/Animation/NS_EnemyMotionWarp.h"

#include "Kismet/KismetMathLibrary.h"



UNS_EnemyMotionWarp::UNS_EnemyMotionWarp() : Super(),
	TimeElapsed(0.0f), TotalTime(0.0f),
	TargetAlpha(0.0f),
	CurrentLocation(FVector::ZeroVector),
	LastLocation(FVector::ZeroVector),
	InitialMoveDistance(0.0f),
	CalculatedMoveDistance(0.0f),
	TargetLocation(FVector::ZeroVector),
	FinalLocation(FVector::ZeroVector),
	bUseLocalLocation(true),
	bEaseIn(true), bEaseOut(true)
{
	//NotifyColor = FColor(200, 200, 100, 255);
}



void UNS_EnemyMotionWarp::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	TimeElapsed = 0.0f;
	TotalTime = TotalDuration;

	if (!GWorld->HasBegunPlay()) return;
	
	CurrentLocation = MeshComp->GetOwner()->GetActorLocation();
	FinalLocation = bUseLocalLocation ?
		CurrentLocation + MeshComp->GetOwner()->GetActorRotation().RotateVector(TargetLocation) : TargetLocation;

	LastLocation = CurrentLocation;

	InitialMoveDistance = (FinalLocation - CurrentLocation).Size();
	CalculatedMoveDistance = InitialMoveDistance;
	TargetAlpha = 0.0f;

	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}



void UNS_EnemyMotionWarp::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	TimeElapsed += FrameDeltaTime;

	if (TimeElapsed < TotalTime && MeshComp->GetOwner() != nullptr && GWorld->HasBegunPlay())
	{
		CalculateTargetAlpha();


		FVector DeltaLocation = FMath::Lerp(CurrentLocation, FinalLocation, TargetAlpha)
			- LastLocation;


		bool bSuccessed = MeshComp->GetOwner()->SetActorLocation(
			MeshComp->GetOwner()->GetActorLocation() + DeltaLocation,
			true);


		// 만약 장애물에 걸렸다면, 이동 거리 빼기
		if (!bSuccessed)
		{
			CalculatedMoveDistance -= DeltaLocation.Size();
		}


		LastLocation = MeshComp->GetOwner()->GetActorLocation();
	}
	
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}



void UNS_EnemyMotionWarp::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

}



void UNS_EnemyMotionWarp::CalculateTargetAlpha()
{
	float ElapsedPercent = TimeElapsed / TotalTime;

	// 범위는 0.0f ~ 1.0f
	float CalculatedMoveAlpha = CalculatedMoveDistance / InitialMoveDistance;

	if (bEaseIn)
	{
		if (bEaseOut)
		{
			TargetAlpha = UKismetMathLibrary::FInterpEaseInOut(0.0f, CalculatedMoveAlpha, ElapsedPercent, 2.0f);
		}
		else
		{
			TargetAlpha = UKismetMathLibrary::Lerp(0.0f, CalculatedMoveAlpha, ElapsedPercent * ElapsedPercent);
		}
	}
	else
	{
		if (bEaseOut)
		{
			TargetAlpha = UKismetMathLibrary::Lerp(0.0f, CalculatedMoveAlpha, FMath::Pow(ElapsedPercent, 0.5f));
		}
		else
		{
			TargetAlpha = UKismetMathLibrary::Lerp(0.0f, CalculatedMoveAlpha, ElapsedPercent);
		}
	}
}
