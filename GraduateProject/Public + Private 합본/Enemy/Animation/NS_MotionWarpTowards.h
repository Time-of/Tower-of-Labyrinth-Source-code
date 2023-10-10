
#pragma once

#include "CoreMinimal.h"
#include "Enemy/Animation/NS_EnemyMotionWarp.h"
#include "NS_MotionWarpTowards.generated.h"



/**
 * @author 이성수
 * @brief 회전값을 도중에 변경해도 새로 목표 지점을 갱신하여 해당 방향으로 이동하는 클래스입니다.
 * @details bUseLocalLocation을 사용하는 경우에만 작동
 * @since 2023-05-17
 */
UCLASS()
class GRADUATEPROJECT_API UNS_MotionWarpTowards : public UNS_EnemyMotionWarp
{
	GENERATED_BODY()

public:
	UNS_MotionWarpTowards();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override
	{
		return TEXT("갱신 방식 모션 워프");
	}



protected:
	FVector SourceFinalLocation;

	FRotator SourceRotation;
};
