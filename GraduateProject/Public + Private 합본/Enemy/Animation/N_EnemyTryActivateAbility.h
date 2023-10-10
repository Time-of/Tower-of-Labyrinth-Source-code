
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "N_EnemyTryActivateAbility.generated.h"



/**
 * @author 이성수
 * @brief 적이 어빌리티 사용을 시도하는 애니메이션 노티파이입니다.
 * @see Notify() 참고.
 * @since 2023-02-03
 */
UCLASS()
class GRADUATEPROJECT_API UN_EnemyTryActivateAbility : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UN_EnemyTryActivateAbility();

	/**
	* @brief 성공 여부에 관계없이 어빌리티 사용을 시도합니다.
	*/
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;



protected:
	/**
	* @brief 발동을 시도할 어빌리티 클래스
	*/
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameplayAbility> AbilityClassToActivate;

	UPROPERTY(EditAnywhere)
	bool bPrintDebugMessage;
};
