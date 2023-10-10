
#include "Factory/ProjectileSimpleFactory.h"

#include "CharacterBase.h"
#include "Enemy/EnemyCharacterBase.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile/ProjectileBase.h"

#include "GameplayEffect.h"
#include "Kismet/GameplayStatics.h"



AProjectileBase* UProjectileSimpleFactory::SpawnProjectileAtLocation(AActor* WorldContext, const TSubclassOf<AProjectileBase> ProjectileToSpawn, const FVector& SpawnLocation, const FRotator& SpawnRotation, const TArray<struct FGameplayEffectSpecHandle>& EffectSpecHandlesToApply, APawn* Instigator, USceneComponent* HomingTargetComp)
{
	FTransform SpawnTransform(SpawnRotation, SpawnLocation);

	AProjectileBase* SpawnedProjectile = WorldContext->GetWorld()->SpawnActorDeferred<AProjectileBase>(ProjectileToSpawn, SpawnTransform, Instigator, Instigator, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	SpawnedProjectile->SetInstigator(Instigator);
	SpawnedProjectile->SetIgnoreInstigator();
	SpawnedProjectile->SetEffectSpecHandlesToApply(EffectSpecHandlesToApply);
	SpawnedProjectile->GetProjectileMovement()->HomingTargetComponent = HomingTargetComp;
	UGameplayStatics::FinishSpawningActor(SpawnedProjectile, SpawnTransform);

	return SpawnedProjectile;
}
