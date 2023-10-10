
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayHelperLibrary.generated.h"



/**
 * @author 이성수
 * @brief 게임플레이 기능을 구현하는 데 필요한 기능을 모아 놓은 블루프린트 함수 라이브러리 클래스입니다.
 * @since 2023-05-04
 */
UCLASS()
class GRADUATEPROJECT_API UGameplayHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	* @brief 간단한 넉백을 플레이어 캐릭터에게 적용합니다.
	* @param FromActor 이 액터로부터 방향을 결정합니다.
	* @param KnockbackPower 넉백 파워입니다.
	* @param KnockbackAirRate 얼마 만큼의 비율로 Z축에 넉백 파워를 적용할지를 결정합니다. (0.0 ~ 1.0)
	*/
	UFUNCTION(BlueprintCallable, Category = "GameplayHelperLibrary|Character")
	static void ApplySimpleKnockbackToPC(AActor* FromActor, float KnockbackPower, float KnockbackAirRate = 0.2f);



	/**
	* @brief 플레이어 캐릭터에게 거리 기반 넉백을 적용합니다.
	* @details 거리가 멀 수록 약한 넉백을 적용받습니다.
	* @param FromActor 이 액터로부터 방향을 결정합니다.
	* @param KnockbackPower 넉백 파워입니다.
	* @param KnockbackAirRate 얼마 만큼의 비율로 Z축에 넉백 파워를 적용할지를 결정합니다. (0.0 ~ 1.0)
	*/
	UFUNCTION(BlueprintCallable, Category = "GameplayHelperLibrary|Character")
	static void ApplyDistanceBasedKnockbackToPC(AActor* FromActor, float KnockbackPower, float KnockbackAirRate = 0.2f);



	/**
	* @brief HitResult에 기반하여 히트 VFX(이펙트) 및 사운드를 스폰합니다.
	* @details HitResult의 적중 지점으로부터 약간의 무작위 위치 델타 지점에 무작위 로테이션으로 히트 이펙트를 스폰합니다.
	* @param WorldContext 현재 월드를 가져오기 위해 필요한 컨텍스트로, 일반적으로 실행자 액터를 연결하면 됩니다.
	* @param HitResult 히트 결과 구조체를 넣습니다.
	* @param HitSfx 히트 사운드
	* @param HitVfx 히트 이펙트
	* @param ScaleVector 히트 이펙트의 스케일을 결정합니다.
	*/
	UFUNCTION(BlueprintCallable, Category = "GameplayHelperLibrary|Character")
	static void SpawnHitEffects(AActor* WorldContext, AActor* TargetActor, USoundBase* HitSfx, class UNiagaraSystem* HitVfx, FVector VfxScaleVector = FVector(1.0f, 1.0f, 1.0f));



	UFUNCTION(BlueprintCallable, Category = "GameplayHelperLibrary|Character")
	static void SpawnHitEffectsParticle(AActor* WorldContext, AActor* TargetActor, USoundBase* HitSfx, UParticleSystem* HitVfx, FVector VfxScaleVector = FVector(1.0f, 1.0f, 1.0f));



	/**
	* @brief 다음 레벨을 엽니다.
	* @see GISS_ShuffleLevel 참고.
	* @param WorldContext 현재 월드를 가져오기 위해 필요한 컨텍스트 액터
	*/
	UFUNCTION(BlueprintCallable, Category = "GameplayHelperLibrary|Level")
	static void OpenNextLevel(AActor* WorldContext);



	UFUNCTION(BlueprintCallable, Category = "GameplayHelperLibrary|Character")
	static class APlayerCharacterBase* GetPlayerCharacter(AActor* WorldContext);
};
