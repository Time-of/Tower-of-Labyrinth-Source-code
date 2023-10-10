

#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "Abilities/GameplayAbilityTypes.h"

#include "NS_EnemyAttack.generated.h"



/**
 * @author 이성수
 * @brief 노티파이 스테이트 지속기간 동안 Sphere 트레이스를 사용하여 공격합니다.
 * @details DamageMult 변수는 피해 배율 변수입니다.
 * @since 2023-01-31
 */
UCLASS(BlueprintType)
class GRADUATEPROJECT_API UNS_EnemyAttack : public UAnimNotifyState
{
	GENERATED_BODY()

	UNS_EnemyAttack();

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override
	{
		return TEXT("트레이스 사용한 공격");
	}

	/**
	* @brief 여기에서 트레이스와 관련 처리를 수행합니다.
	*/
	void TryTrace(const USkeletalMeshComponent* MeshComp);



private:
	//UPROPERTY()
	//mutable class AEnemyCharacterBase* OwnerEnemy;

	/** @brief 피해 배율 커브 테이블 핸들 */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", EditCondition = "!bUseStrengthToDamage"))
	FCurveTableRowHandle DamageCurveTable;

	/** @brief true라면 힘 스탯을 그대로 공격력으로 사용합니다. */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bUseStrengthToDamage;

	float DamageMultRead;

	mutable float TimeElapsed;

	/**
	* @brief 틱이 일어나는 주기입니다.
	*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float TickDelay;

	/**
	* @brief Sphere 트레이스를 수행할 때 사용할 구체의 크기입니다.
	*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float SphereRadius;

	/**
	* @brief Sphere 트레이스를 사용할 소켓 이름입니다.
	*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName SocketName;

	/**
	* @brief true인 경우 트레이스의 디버그를 그립니다.
	*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bDrawDebug;

	mutable FVector PrevLocation;

	/**
	* @brief 트레이스를 무시할 액터입니다.
	* @details 기본적으로 OwnerEnemy가 포함되며, NotifyBegin()에서 초기화합니다.
	* @details 대상이 여러 번 히트되지 않도록 하는 역할을 수행합니다.
	*/
	mutable TArray<AActor*> IgnoreActors;
};
