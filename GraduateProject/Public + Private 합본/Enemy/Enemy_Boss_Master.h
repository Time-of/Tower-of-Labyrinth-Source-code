
#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Enemy_Boss_Master.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPhase2Entered);



/**
 * @author 이성수
 * @brief 적 보스 캐릭터 베이스 클래스입니다.
 * @since 2023-01-31
 */
UCLASS()
class GRADUATEPROJECT_API AEnemy_Boss_Master : public AEnemyCharacterBase
{
	GENERATED_BODY()

public:
	AEnemy_Boss_Master();

	/**
	 * @brief 중경직입니다. 특정 몽타주를 재생시킵니다.
	 * @see StiffMedium_Montage 참고.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy|State")
	void Stiff_Medium();

	/**
	 * @brief 2페이즈 진입 시 사용될 VFX를 가져옵니다.
	 * @details 나이아가라와 파티클 이미터 중 유연한 사용을 위해 만든 기능입니다.
	 * @details 현재 게임플레이 큐에서 사용하고 있습니다.
	 * 
	 * @param[out] bIsParticleEmitter 파티클 이미터를 사용하고있는지의 여부입니다.
	 * @param[out] OutNiagara EnterPhase2_Niagara입니다.
	 * @param[out] OutParticle EnterPhase2_Particle입니다.
	 * @return GetActorLocation() + EnterPhase2_VFX_Offset를 반환합니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Enemy|Pattern|Phase")
	FVector GetPhase2Vfx(bool& bIsParticleEmitter, class UNiagaraSystem*& OutNiagara, UParticleSystem*& OutParticle);

	/**
	 * @brief 2페이즈 진입 조건을 체크하고, 조건이 맞다면 2페이즈로 진입시킵니다. 한 번만 발동합니다.
	 * @details 조건이 맞다면 (현재: 체력이 50% 이하), 이 함수를 실행했을 때 StartPhase2AbilityEachTag의 각 태그 별 어빌리티를 실행합니다.
	 * @see StartPhase2AbilityEachTag 및 bPhase2Entered 참고.
	 */
	UFUNCTION(BlueprintCallable, Category = "Enemy|Pattern|Phase")
	virtual void CheckPhase2();
	


	void OnHealthChanged_Implementation(float DeltaHealth, const FGameplayTagContainer& SourceTags) override;

	void OnGuardChanged_Implementation(float DeltaGuard, const FGameplayTagContainer& SourceTags) override;

	void OnDamaged_Implementation(float DamageAmount, const FHitResult& HitResult, const FGameplayTagContainer& DamageTags, ACharacterBase* InstigatorCharacter, AActor* DamageCauser) override;

	void UpdateHealthbar_Implementation(bool bShowHealthbar) override;

	UUserWidget* GetHealthbarWidgetObject() const
	{
		return HealthbarWidgetComp->GetUserWidgetObject();
	}

	

	UPROPERTY(BlueprintAssignable, Category = "Enemy|Pattern|Phase")
	FOnPhase2Entered OnPhase2EnteredDelegate;



protected:
	void BeginPlay() override;

	void DieAction() override;

	/** @brief 사망 시 해금할 아이템 정보입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Death")
	TArray<FPrimaryAssetId> UnlockItemsOnDeath;

	/**
	* @brief 체력바 위젯이 뷰포트에 추가되었는지 여부입니다.
	* @see UpdateHealthbar_Implementation() 참고.
	*/
	bool bHealthbarWidgetAddedToViewport;

	/**
	* @brief 중경직에 사용되는 몽타주입니다.
	* @see Stiff_Medium() 참고.
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Enemy|State")
	UAnimMontage* StiffMedium_Montage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy|State")
	FName StiffMediumMontageSectionName;

	/**
	* @brief 2페이즈에 들어갔는지 여부
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy|Pattern|Phase")
	bool bPhase2Entered;

	/**
	* @brief 2페이즈가 시작될 때 적용될 어빌리티의 태그 목록입니다.
	* @details 모두 따로 적용됩니다.
	*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enemy|Pattern|Phase")
	TArray<TSubclassOf<UGameplayAbility>> AbilityClassesToUseOnPhase2_Started;

	/**
	* @brief 가드가 0 이하가 되면 발동하는 이펙트입니다.
	* @see OnGuardChanged_Implementation() 참고.
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Enemy|Pattern|Phase")
	TSubclassOf<UGameplayEffect> RefillGuardEffect;

	/**
	* @brief 2페이즈 진입 시 파티클 시스템을 사용하는지 여부입니다.
	* @details false인 경우 나이아가라 시스템을 사용합니다.
	* @see GetPhase2Vfx() 참고.
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Enemy|Pattern|Phase")
	bool bUseParticleSystemForEnterPhase2Vfx;

	/**
	* @brief 2페이즈 진입 시 사용할 나이아가라 시스템
	* @see GetPhase2Vfx() 참고.
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Enemy|Pattern|Phase", meta = (EditCondition = "!bUseParticleSystemForEnterPhase2Vfx"))
	class UNiagaraSystem* EnterPhase2_Niagara;

	/**
	* @brief 2페이즈 진입 시 사용할 파티클
	* @see GetPhase2Vfx() 참고.
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Enemy|Pattern|Phase", meta = (EditCondition = "bUseParticleSystemForEnterPhase2Vfx"))
	UParticleSystem* EnterPhase2_Particle;

	/**
	* @brief 2페이즈 진입 이펙트의 위치를 조정할 오프셋
	* @see GetPhase2Vfx() 참고.
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Enemy|Pattern|Phase")
	FVector EnterPhase2_VFX_Offset;

	/** @brief 2페이즈 진입 체력 비율 (기본: 0.5f) */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enemy|Pattern|Phase")
	float Phase2EnterHealthRate;
};
