
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectileBase.generated.h"



/**
 * @author 이성수
 * @brief 투사체 베이스 클래스입니다.
 * @since 2023-03-18
 */
UCLASS()
class GRADUATEPROJECT_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

	void SetEffectSpecHandlesToApply(const TArray<struct FGameplayEffectSpecHandle>& NewEffectSpecHandlesToApply);

	class UProjectileMovementComponent* GetProjectileMovement() const;

	/** @brief 투사체를 발사한 액터를 무시할 것인지 설정하는 기능 */
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SetIgnoreInstigator(bool NewActive);

	void SetIgnoreInstigator();



protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	// 컨스트럭션 스크립트 오버라이드
	virtual void OnConstruction(const FTransform& Transform) override;



	UFUNCTION()
	void OnCollisionSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnCollsionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



	/** @brief true라면 오버랩 방식으로 사용, false라면 Hit 방식으로 사용 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile")
	bool bUseOverlapMethod;

	/** @brief 이 투사체를 발사한 액터 무시 여부 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile")
	bool bIgnoreInstigator;

	/** @brief 플레이어에게 영향을 주는지 여부 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile")
	bool bAffectsPlayer;

	/** @brief 몬스터에게 영향을 주는지 여부 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Projectile")
	bool bAffectsMonster;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	TArray<struct FGameplayEffectSpecHandle> EffectSpecHandlesToApply;



	/** @brief 투사체가 삭제될 때 스폰될 파티클 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile|VFX")
	UParticleSystem* SpawnOnDestroyedParticle;

	/** @brief 투사체가 삭제될 때 스폰될 나이아가라 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile|VFX")
	class UNiagaraSystem* SpawnOnDestroyedNiagara;

	/** @brief 투사체가 삭제될 때 사용될 카메라 흔들기 클래스 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile|VFX")
	TSubclassOf<UCameraShakeBase> CamShakeOnDestroyedClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile|VFX")
	float CamShakeRadius;


	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USphereComponent* CollisionSphereComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComp;
};
