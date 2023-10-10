
#include "Enemy/Enemy_Boss_Fighter.h"

#include "BlueprintFunctionLibrary/UnlockCraftItemHelper.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Abilities/GameplayAbilityBase.h"
#include "GameInstance/GISS_InteractableNpcData.h"
#include "Widget/Interface/BossHealthbarWidgetHandler.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



AEnemy_Boss_Fighter::AEnemy_Boss_Fighter() : Super(),
	bPhase3Entered(false),
	Phase3EnterHealthRate(0.4f),
	bUsingSword(false),
	AblazeEffectClass(nullptr),
	ElectricEffectClass(nullptr)
{

}



void AEnemy_Boss_Fighter::CheckPhase2()
{
	if (!bPhase2Entered && GetHealth() <= GetMaxHealth() * Phase2EnterHealthRate)
	{
		bPhase2Entered = true;

		for (TSubclassOf<UGameplayAbility> AbilityToActivate : AbilityClassesToUseOnPhase2_Started)
		{
			AbilitySystemComponent->TryActivateAbilityByClass(AbilityToActivate);
		}

		OnPhase2EnteredDelegate.Broadcast();

		SetUsingSword(false);
	}
	else if (!bPhase3Entered && GetHealth() <= GetMaxHealth() * Phase3EnterHealthRate)
	{
		bPhase3Entered = true;

		OnPhase3EnteredDelegate.Broadcast();
	}
}



void AEnemy_Boss_Fighter::SetUsingSword(bool NewActive)
{
	if (!bPhase2Entered && !bPhase3Entered) return;

	bUsingSword = NewActive;

	SetAdditionalAttackEffectOnTarget(NewActive ? ElectricEffectClass : AblazeEffectClass);

	PostUsingSwordChanged(bUsingSword);
}
