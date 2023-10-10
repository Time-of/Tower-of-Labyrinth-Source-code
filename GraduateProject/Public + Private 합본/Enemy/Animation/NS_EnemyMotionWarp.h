
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NS_EnemyMotionWarp.generated.h"



/**
 * @author 이성수
 * @brief 노티파이 스테이트 동안 적 캐릭터의 위치를 원하는 로컬 위치로 이동시킵니다.
 * @since 2023-01-31
 */
UCLASS()
class GRADUATEPROJECT_API UNS_EnemyMotionWarp : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UNS_EnemyMotionWarp();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override
	{
		return TEXT("Enemy 모션 워프");
	}

protected:
	void CalculateTargetAlpha();



	float TimeElapsed;

	float TotalTime;

	float TargetAlpha;

	FVector CurrentLocation;

	FVector LastLocation;

	/** @brief 원래 이동해야 할 거리 */
	float InitialMoveDistance;

	/** @brief 최종적으로 이동해야 할 거리 (중간에 장애물을 만나 이동하지 못하는 경우 이동해야 할 거리를 줄이기 위해 사용) */
	float CalculatedMoveDistance;

	/**
	* @brief 목표 위치
	* @details bUseLocalLocation이라면 로컬 좌표, 아니라면 월드 좌표
	* @details 로컬 좌표인 경우 로컬 회전값이 적용됩니다.
	*/ 
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector TargetLocation;

	/**
	* @brief 목표 위치값이 계산된 최종 위치
	*/
	FVector FinalLocation;

	/**
	* @brief 로컬 위치를 사용할 것인지 여부
	*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bUseLocalLocation;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bEaseIn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bEaseOut;
};
