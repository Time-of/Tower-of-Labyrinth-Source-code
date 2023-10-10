
#include "Factory/EnemySimpleFactory.h"

#include "Enemy/EnemyCharacterBase.h"

#include "Kismet/GameplayStatics.h"



AEnemyCharacterBase* UEnemySimpleFactory::SpawnEnemy(const AActor* WorldContext, TSubclassOf<class AEnemyCharacterBase> EnemyClassToSpawn, const FVector& Location, const FRotator& Rotation, APawn* Instigator, AActor* Owner, const FScriptDelegate& EnemyDiedCallback)
{
	if (!IsValid(WorldContext) || !IsValid(EnemyClassToSpawn)) return nullptr;

	FTransform SpawnTransform(Rotation, Location);

	AEnemyCharacterBase* SpawnedEnemy = WorldContext->GetWorld()->SpawnActorDeferred<AEnemyCharacterBase>(EnemyClassToSpawn, SpawnTransform, Owner, Instigator, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	SpawnedEnemy->SetInstigator(Instigator);
	SpawnedEnemy->OnEnemyDied.AddUnique(EnemyDiedCallback);
	UGameplayStatics::FinishSpawningActor(SpawnedEnemy, SpawnTransform);

	return SpawnedEnemy;
}
