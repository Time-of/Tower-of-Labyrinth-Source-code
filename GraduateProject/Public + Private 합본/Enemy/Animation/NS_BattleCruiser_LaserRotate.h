
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NS_BattleCruiser_LaserRotate.generated.h"



/**
 * @author 이성수
 * @brief 전투순양함 보스가 TargetActor를 바라볼 때 사용하는 노티파이 스테이트 클래스입니다.
 * @details 어빌리티 종료 시 회전값 정상화시킬 것!!
 * @since 2023-04-20
 */
UCLASS()
class GRADUATEPROJECT_API UNS_BattleCruiser_LaserRotate : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UNS_BattleCruiser_LaserRotate();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	const FRotator InternalGetTargetRotation(USkeletalMeshComponent* MeshComp) const;

	virtual FString GetNotifyName_Implementation() const override
	{
		return TEXT("전투순양함 보스: TargetActor 바라보기 (Pitch 회전 포함)");
	}



private:
	//UPROPERTY()
	//mutable class AEnemyCharacterBase* OwnerEnemy;

	//UPROPERTY()
	//AActor* TargetActorRef;

	float TotalTime;

	float ElapsedTime;
};
