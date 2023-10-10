
#include "Enemy/Abilities/Boss/Plant/GA_Cpp_Boss_Plant_SpawnRoots.h"

#include "Enemy/Enemy_Boss_Plant.h"
#include "Enemy/Interface/BossPlantHandler.h"
#include "Factory/EnemySimpleFactory.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"



UGA_Cpp_Boss_Plant_SpawnRoots::UGA_Cpp_Boss_Plant_SpawnRoots() : Super(),
	RootEnemyClass(nullptr)
{
	static ConstructorHelpers::FClassFinder<AEnemyCharacterBase> DefaultRootEnemyClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Enemy_Boss/Plant/Enemy_Plant_Root");
	if (DefaultRootEnemyClass.Succeeded())
	{
		RootEnemyClass = DefaultRootEnemyClass.Class;
	}
}



void UGA_Cpp_Boss_Plant_SpawnRoots::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}

	if (AEnemy_Boss_Plant* PlantBoss = Cast<AEnemy_Boss_Plant>(GetAvatarActorFromActorInfo()))
	{
		TArray<AActor*> IgnoreActors { PlantBoss };
		FHitResult HitResult;

		for (const FVector& RootLocation : PlantBoss->GetRootSpawnLocalLocations())
		{
			bool bTraceHit = UKismetSystemLibrary::SphereTraceSingle(PlantBoss,
				PlantBoss->GetActorLocation() + RootLocation,
				PlantBoss->GetActorLocation() + RootLocation + FVector::DownVector * 300.0f,
				16.0f,
				UEngineTypes::ConvertToTraceType(ECC_Visibility),
				false, IgnoreActors,
				EDrawDebugTrace::None, HitResult, true);

			FVector SpawnLocation = bTraceHit ? HitResult.Location : HitResult.TraceEnd;

			//FScriptDelegate OnEnemyDiedEvent;
			//OnEnemyDiedEvent.BindUFunction(this, FName("OnRootEnemyDied"));

			//UEnemySimpleFactory::SpawnEnemy(PlantBoss, RootEnemyClass, SpawnLocation, FRotator::ZeroRotator,
			//	PlantBoss, PlantBoss, OnEnemyDiedEvent);

			FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);

			AEnemyCharacterBase* SpawnedEnemy = PlantBoss->GetWorld()->SpawnActorDeferred<AEnemyCharacterBase>(
				RootEnemyClass,
				SpawnTransform,
				PlantBoss,
				PlantBoss,
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
				);

			SpawnedEnemy->SetInstigator(PlantBoss);
			SpawnedEnemy->OnEnemyDied.AddUniqueDynamic(PlantBoss, &AEnemy_Boss_Plant::OnPlantRootDied);
			UGameplayStatics::FinishSpawningActor(SpawnedEnemy, SpawnTransform);
		}

		//EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
	else CancelAbility(Handle, ActorInfo, ActivationInfo, false);
}
