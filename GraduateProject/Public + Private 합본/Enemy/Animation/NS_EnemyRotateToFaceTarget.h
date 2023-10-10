
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NS_EnemyRotateToFaceTarget.generated.h"



UENUM(BlueprintType)
enum class EEnemyRotateToFaceType : uint8
{
	FollowTowards UMETA(DisplayName = "쭉 따라가기"),
	Lerp UMETA(DisplayName = "지속 시간동안 선형 보간"),
	Interp UMETA(DisplayName = "일정 속도(InterpSpeed)로 보간")
};



/**
 * @author 이성수
 * @brief 지속 기간동안 적이 TargetActor 블랙보드 키를 바라보게 되는 노티파이 스테이트입니다.
 * @details RotateStrategy 열거형에 따라 방법을 다르게 선택하는 **간략화된** 스트래티지 패턴을 사용했습니다.
 * @since 2023-02-08
 */
UCLASS()
class GRADUATEPROJECT_API UNS_EnemyRotateToFaceTarget : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UNS_EnemyRotateToFaceTarget();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override
	{
		return TEXT("TargetActor 바라보기");
	}



private:
	void FollowTowardsStrategy(USkeletalMeshComponent* MeshComp, float FrameDeltaTime);

	void LerpStrategy(USkeletalMeshComponent* MeshComp, float FrameDeltaTime);

	void InterpStrategy(USkeletalMeshComponent* MeshComp, float FrameDeltaTime);

	/** @brief 여러 번 쓰이는 선형 보간 기능입니다. */
	void InternalLerpRotation(USkeletalMeshComponent* MeshComp, float FrameDeltaTime, const FRotator& FromRotation);

	const FRotator InternalGetTargetRotation(USkeletalMeshComponent* MeshComp) const;

	void SetRotation(USkeletalMeshComponent* MeshComp, const FRotator& Rotation);



	//UPROPERTY()
	//mutable class AEnemyCharacterBase* OwnerEnemy;

	//UPROPERTY()
	//AActor* TargetActorRef;

	float TotalTime;

	float ElapsedTime;

	FRotator SourceRotation;

	UPROPERTY(EditAnywhere)
	EEnemyRotateToFaceType RotateStrategy;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "RotateStrategy == EEnemyRotateToFaceType::Interp"))
	float InterpSpeed;

	/** @brief 바라봐야 하는 Yaw 회전값이 너무 크다면 회전하지 않기 */
	UPROPERTY(EditAnywhere)
	bool bDontRotateWhenYawDeltaTooLarge;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bDontRotateWhenYawDeltaTooLarge"))
	float AbsYawDeltaThreshold;
};
