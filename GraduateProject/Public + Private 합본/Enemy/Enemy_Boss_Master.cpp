
#include "Enemy/Enemy_Boss_Master.h"

#include "BlueprintFunctionLibrary/UnlockCraftItemHelper.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Abilities/GameplayAbilityBase.h"
#include "GameInstance/GISS_InteractableNpcData.h"
#include "Widget/Interface/BossHealthbarWidgetHandler.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



AEnemy_Boss_Master::AEnemy_Boss_Master() : Super(),
	UnlockItemsOnDeath(),
	bHealthbarWidgetAddedToViewport(false),
	StiffMedium_Montage(nullptr),
	StiffMediumMontageSectionName(TEXT("NAME_None")),
	bPhase2Entered(false),
	AbilityClassesToUseOnPhase2_Started(),
	RefillGuardEffect(nullptr),
	bUseParticleSystemForEnterPhase2Vfx(false),
	EnterPhase2_Niagara(nullptr),
	EnterPhase2_Particle(nullptr),
	EnterPhase2_VFX_Offset(FVector::ZeroVector),
	Phase2EnterHealthRate(0.5f)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultHealthbarClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Widget/WBP_Boss_Healthbar");

	if (DefaultHealthbarClass.Succeeded())
	{
		HealthbarWidgetComp->SetWidgetClass(DefaultHealthbarClass.Class);
	}

	static ConstructorHelpers::FClassFinder<UGameplayEffect> DefaultRefillGuardEffectClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Effects/Boss/GE_RefillGuard");

	if (DefaultRefillGuardEffectClass.Succeeded())
	{
		RefillGuardEffect = DefaultRefillGuardEffectClass.Class;
	}

	HealthbarWidgetComp->SetVisibility(false);

	bHaveToDoPatrol = false;
	bForcePerceptionOnHit = false;
}



void AEnemy_Boss_Master::Stiff_Medium_Implementation()
{
	CancelCancelableAbilities();
	
	PlayAnimMontage(StiffMedium_Montage, 1.0f, StiffMediumMontageSectionName);
}



FVector AEnemy_Boss_Master::GetPhase2Vfx(bool& bIsParticleEmitter, UNiagaraSystem*& OutNiagara, UParticleSystem*& OutParticle)
{
	bIsParticleEmitter = bUseParticleSystemForEnterPhase2Vfx;
	OutParticle = EnterPhase2_Particle;
	OutNiagara = EnterPhase2_Niagara;
	
	return GetActorLocation() + EnterPhase2_VFX_Offset;
}



void AEnemy_Boss_Master::CheckPhase2()
{
	if (!bPhase2Entered && GetHealth() <= GetMaxHealth() * Phase2EnterHealthRate)
	{
		bPhase2Entered = true;

		for (TSubclassOf<UGameplayAbility> AbilityToActivate : AbilityClassesToUseOnPhase2_Started)
		{
			AbilitySystemComponent->TryActivateAbilityByClass(AbilityToActivate);
		}

		OnPhase2EnteredDelegate.Broadcast();
	}
}



void AEnemy_Boss_Master::DieAction()
{
	AEnemyCharacterBase::DieAction();

	for (const auto& Item : UnlockItemsOnDeath)
	{
		UUnlockCraftItemHelper::UnlockCraftItem(this, Item);
	}
}



void AEnemy_Boss_Master::OnHealthChanged_Implementation(float DeltaHealth, const FGameplayTagContainer& SourceTags)
{
	if (DeltaHealth < 0.0f && GetHealth() > 0.0f)
	{
		ShowDamageText(DeltaHealth);
	}
	
	if (!IsAlive())
	{
		HandleDie();
		return;
	}

	if (bShowHealthbarWidgetOnHealthChanged) UpdateHealthbar(true);

	CheckPhase2();
}



void AEnemy_Boss_Master::OnGuardChanged_Implementation(float DeltaGuard, const FGameplayTagContainer& SourceTags)
{
	UUserWidget* HealthbarWidget = GetHealthbarWidgetObject();

	if (HealthbarWidget != nullptr && HealthbarWidget->Implements<UBossHealthbarWidgetHandler>())
	{
		IBossHealthbarWidgetHandler::Execute_SetGuardPercent(HealthbarWidget, FMath::Max(0.0f, GetGuard() / DefaultGuard));
	}

	if (GetGuard() <= 0.0f && AbilitySystemComponent != nullptr && IsValid(RefillGuardEffect) &&
		AbilitySystemComponent->GetGameplayEffectCount(RefillGuardEffect, AbilitySystemComponent) <= 0)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(RefillGuardEffect, GetCharacterLevel(),
			AbilitySystemComponent->MakeEffectContext());

		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}



void AEnemy_Boss_Master::OnDamaged_Implementation(float DamageAmount, const FHitResult& HitResult, const FGameplayTagContainer& DamageTags, ACharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	if (GetGuard() > 0.0f)
	{
		Stiff_Light();
	}
	else if (AbilitySystemComponent->GetTagCount(FGameplayTag::RequestGameplayTag(TEXT("State.SuperArmor"))) <= 0)
	{
		const FRotator LookAtRotation = DamageCauser != nullptr ? UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageCauser->GetActorLocation()) : 
			GetActorRotation();

		const FRotator TargetRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);

		SetActorRotation(TargetRotation);

		Stiff_Medium();
	}
}



void AEnemy_Boss_Master::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* HealthbarWidget = HealthbarWidgetComp->GetUserWidgetObject();

	if (HealthbarWidget != nullptr && HealthbarWidget->Implements<UBossHealthbarWidgetHandler>())
	{
		IBossHealthbarWidgetHandler::Execute_SetBossNameText(HealthbarWidget, DisplayEnemyName);
	}
}



void AEnemy_Boss_Master::UpdateHealthbar_Implementation(bool bShowHealthbar)
{
	if (UUserWidget* HealthbarWidget = GetHealthbarWidgetObject())
	{
		if (bShowHealthbar)
		{
			if (!bHealthbarWidgetAddedToViewport)
			{
				bHealthbarWidgetAddedToViewport = true;
				HealthbarWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
				HealthbarWidget->AddToViewport(10);
			}

			if (HealthbarWidget->Implements<UBossHealthbarWidgetHandler>())
			{
				IBossHealthbarWidgetHandler::Execute_SetHealthPercent(HealthbarWidget, GetHealth() / GetMaxHealth());
			}
		}
		else
		{
			bHealthbarWidgetAddedToViewport = false;
			HealthbarWidget->SetVisibility(ESlateVisibility::Collapsed);
			HealthbarWidget->RemoveFromParent();
		}
	}
}