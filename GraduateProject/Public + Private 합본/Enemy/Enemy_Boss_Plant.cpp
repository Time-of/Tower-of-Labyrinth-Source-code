
#include "Enemy/Enemy_Boss_Plant.h"

#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Interface/BossPlantHandler.h"
#include "Factory/EnemySimpleFactory.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"



AEnemy_Boss_Plant::AEnemy_Boss_Plant() : Super(),
	DecreaseGuardEffectClass(nullptr),
	RootSpawnLocationArray()
{
	static ConstructorHelpers::FClassFinder<UGameplayEffect> DefaultDecreaseGuardEffectClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Effects/Boss/Plant/GE_Boss_Plant_Phase2_DecreaseGuard");
	if (DefaultDecreaseGuardEffectClass.Succeeded())
	{
		DecreaseGuardEffectClass = DefaultDecreaseGuardEffectClass.Class;
	}
}



void AEnemy_Boss_Plant::OnPlantRootDied(AEnemyCharacterBase* Enemy)
{
	if (!IsValid(AbilitySystemComponent) || !IsValid(this)) return;

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		DecreaseGuardEffectClass,
		1.0f,
		AbilitySystemComponent->MakeEffectContext());

	// 대상의 받는 가드 피해량 배율을 적용하지 않음 (즉 그대로 배율로 사용할 것)
	SpecHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle,
		FGameplayTag::RequestGameplayTag(TEXT("Data.DirectDamageToGuard.UseGuardDamageTakenMult")),
		0.0f);

	// 가드 피해량은 가드 최대치의 34%
	SpecHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle,
		FGameplayTag::RequestGameplayTag(TEXT("Data.DamageMult")),
		0.34f);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}



TArray<FVector> AEnemy_Boss_Plant::GetRootSpawnLocalLocations() const
{
	return RootSpawnLocationArray;
}
