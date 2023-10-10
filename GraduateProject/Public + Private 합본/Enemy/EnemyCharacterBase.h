
#pragma once

#include "CoreMinimal.h"

#include "CharacterBase.h"
#include "Enemy/Interface/EnemyHandler.h"

#include "Components/TimelineComponent.h"

#include "EnemyCharacterBase.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyDiedDelegate, AEnemyCharacterBase*, EnemyCharacter);



/**
 * @author 이성수
 * @brief 적 캐릭터 베이스 클래스입니다.
 * @since 2023-01-31
 */
UCLASS()
class GRADUATEPROJECT_API AEnemyCharacterBase : public ACharacterBase, public IEnemyHandler
{
	GENERATED_BODY()

public:
	AEnemyCharacterBase();

	/**
	 * @brief 약경직입니다. 캐릭터를 뒤쪽으로 미세하게 발사시킵니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Enemy|State")
	void Stiff_Light();

	void SetGuardToFull_Implementation() override;

	TSubclassOf<UGameplayEffect> GetAdditionalAttackEffectOnTarget_Implementation() const override;

	virtual void OnHealthChanged_Implementation(float DeltaHealth, const FGameplayTagContainer& SourceTags) override;

	UFUNCTION()
	void OnHideHealthbarTimelineUpdate(float Value);

	/** @brief HideHealthbarTimeline이 Finished되었을 때 호출 */
	UFUNCTION()
	void OnHideHealthbarTimelineFinished();

	/** @brief DeathFadeoutTimeline이 Update될 때 호출 */
	UFUNCTION(BlueprintNativeEvent, Category = "Enemy|Death")
	void OnDeathFadeoutTimelineUpdate(float Value);

	/** @brief DeathAnimTimeline이 Finished되었을 때 호출 */
	UFUNCTION()
	void OnDeathFadeoutTimelineFinished();

	void HandleCancelCancelableAbilities();



	UPROPERTY(BlueprintAssignable, Category = "Enemy|State")
	FEnemyDiedDelegate OnEnemyDied;

	

protected:
	virtual void BeginPlay() override;

	virtual void DieAction() override;

	void InitializeDefaultAttribute() final;

	void OnDamaged_Implementation(float DamageAmount, const FHitResult& HitResult, const FGameplayTagContainer& DamageTags, ACharacterBase* InstigatorCharacter, AActor* DamageCauser) override;

	/** @brief Ability.Enemy.CanBeCanceled 태그가 붙은 어빌리티를 취소시킵니다. */
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void CancelCancelableAbilities();

	/**
	 * @brief 체력바 위젯을 업데이트합니다.
	 * @see HealthbarWidgetComp 참고.
	 * 
	 * @param bShowHealthbar true라면 보여주고, false라면 숨깁니다.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy|UI")
	void UpdateHealthbar(bool bShowHealthbar = true);

	/**
	 * @brief 피해량 텍스트를 띄웁니다.
	 * @see DamageTextWidgetComponentClass 참고.
	 * 
	 * @param DamageAmount 피해량의 수치입니다.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy|UI")
	void ShowDamageText(float DamageAmount);

	void DropItems();



	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy")
	bool bHaveToDoPatrol;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy")
	bool bForcePerceptionOnHit;

	/** @brief 이미 히트에 의해 인지되었는지 확인 */
	bool bPerceptionedByHit;

	/** @brief 적 캐릭터의 이름입니다. 주로 드랍 테이블 정보를 읽어올 때 사용합니다. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Enemy")
	FName InternalEnemyName;

	/** @brief 적 캐릭터의 실제 보여질 이름입니다. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Enemy")
	FString DisplayEnemyName;

	/**
	* @brief 드랍되는 돈 픽업 하나 당 가질 수 있는 최대 수량입니다.
	* @details 만약 50이고, 드랍되는 돈이 180이라면, 총 4개의 돈 픽업 액터가 생성될 것입니다. (50 * 3, 30 * 1)
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy")
	float MaxMoneyAmountPerPickup;

	/**
	 * @brief 약경직 시 사용될 로컬 발사 속도(방향+힘)입니다.
	 * @see Stiff_Light() 참고.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Enemy|State")
	FVector StiffLight_LocalDirection;

	float DefaultGuard;

	/**
	 * @brief 체력이 변경되었을 때 체력바를 보여줄 것인지 여부입니다.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy|UI")
	bool bShowHealthbarWidgetOnHealthChanged;

	/**
	 * @brief 체력바 위젯 컴포넌트입니다.
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy|UI", meta = (EditCondition = "bShowHealthbarWidgetOnHealthChanged"))
	class UWidgetComponent* HealthbarWidgetComp;

	/** @brief 체력바 위젯을 피해를 입고 일정 시간 후 숨깁니다. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy|UI", meta = (EditCondition = "bShowHealthbarWidgetOnHealthChanged"))
	bool bHideHealthbarOnHitAfterDelay;

	/** @brief 체력바 위젯을 숨기기 위한 타임라인 */
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|UI")
	class UTimelineComponent* HideHealthbarTimeline;

	/** @brief 체력바 위젯 숨기기 타임라인이 진행하게 될 Float 커브 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy|UI")
	UCurveFloat* HideHealthbarTimelineAnimCurveFloat;

	class FOnTimelineFloat HideHealthbarTimelineUpdate;

	class FOnTimelineEvent HideHealthbarTimelineFinished;

	//FTimerHandle HideHealthbarTimerHandle;

	/**
	 * @brief 히트 시 띄워지게 될 피해량 텍스트 위젯 컴포넌트 클래스입니다.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy|UI")
	TSubclassOf<class UWidgetComponent> DamageTextWidgetComponentClass;



	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Enemy|Death")
	USoundBase* DeathSfx;

	/** @brief 사망 시 페이드아웃 타임라인 */
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Death")
	class UTimelineComponent* DeathFadeoutTimeline;

	/** @brief DeathFadeoutTimeline이 진행하게 될 Float 커브 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy|Death")
	UCurveFloat* DeathFadeoutAnimCurveFloat;

	/** @brief DeathFadeoutTimeline이 끝나고 삭제되기까지의 시간 */
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy|Death")
	//float DestroyDelay;

	class FOnTimelineFloat DeathFadeoutAnimTimelineUpdate;

	class FOnTimelineEvent DeathFadeoutAnimTimelineFinished;
};
