
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Enemy/Abilities/AbilitySystemCompBase.h"

#include "ProjectileSimpleFactory.generated.h"




/**
 * @author 이성수
 * @brief 투사체를 쉽게 스폰시키기 위한 심플 팩토리 클래스입니다.
 * @since 2023-03-18
 */
UCLASS()
class GRADUATEPROJECT_API UProjectileSimpleFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "ProjectileSimpleFactory")
	static class AProjectileBase* SpawnProjectileAtLocation(AActor* WorldContext,
		const TSubclassOf<AProjectileBase> ProjectileToSpawn,
		const FVector& SpawnLocation, const FRotator& SpawnRotation,
		const TArray<FGameplayEffectSpecHandle>& EffectSpecHandlesToApply,
		APawn* Instigator, USceneComponent* HomingTargetComp = nullptr);
};
