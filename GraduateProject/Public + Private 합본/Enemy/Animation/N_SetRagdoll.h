
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "N_SetRagdoll.generated.h"



/**
 * @author 이성수
 * @brief 캐릭터를 래그돌 상태로 만드는 애니메이션 노티파이입니다.
 * @see Notify() 참고.
 * @since 2023-02-04
 */
UCLASS()
class GRADUATEPROJECT_API UN_SetRagdoll : public UAnimNotify
{
	GENERATED_BODY()

public:
	UN_SetRagdoll();

	/**
	* @brief bEnableRagdoll 변수에 따라서 래그돌 활성/비활성화합니다.
	*/
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;



protected:
	/**
	* @brief true인 경우 래그돌 활성화, false인 경우 래그돌 비활성화
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bEnableRagdoll;
};
