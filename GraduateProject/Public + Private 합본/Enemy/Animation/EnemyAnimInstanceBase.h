
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstanceBase.generated.h"



/**
 * @author 이성수
 * @brief 적 캐릭터의 애님 인스턴스 베이스 클래스입니다.
 * @since 2023-02-04
 */
UCLASS()
class GRADUATEPROJECT_API UEnemyAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyAnimInstanceBase();

	/**
	* @brief 애니메이션 재생에 필요한 각종 변수들을 업데이트합니다.
	*/
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;



protected:
	UPROPERTY()
	class AEnemyCharacterBase* OwnerEnemy;

	/**
	* @brief 지상 이동속도
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float GroundMoveSpeed;

	/**
	* @brief 떨어지고 있는가 여부
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsFalling;

	/**
	* @brief 이동 방향 (CalculateDirection 사용)
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MoveDirection;

	/**
	* @brief 컨트롤러와 캐릭터 간의 Yaw 회전 델타 값
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float YawDelta;
};
