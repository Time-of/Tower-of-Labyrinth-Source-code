
#include "Enemy/Enemy_Boss_BattleCruiser.h"

#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/State/BossStateMachine_BattleCruiser.h"
#include "Widget/Interface/BossHealthbarWidgetHandler.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



AEnemy_Boss_BattleCruiser::AEnemy_Boss_BattleCruiser() : Super(),
	GroggyDuration(5.0f),
	HitBoxComp(nullptr),
	ForceMoveSpeedZeroEffectClass(nullptr),
	StateMachineComp(nullptr),
	LandTimelineComp(nullptr), LandCurveFloat(nullptr),
	GroggyTimerHandle()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(400.0f);
	GetCapsuleComponent()->SetCapsuleRadius(20.0f);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_WorldDynamic);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);

	HitBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBoxComp->SetupAttachment(GetRootComponent());
	HitBoxComp->SetBoxExtent(FVector(440.0f, 56.0f, 48.0f));
	HitBoxComp->SetCollisionProfileName("Pawn");
	HitBoxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	HitBoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 240.0f, 0.0f);

	static ConstructorHelpers::FClassFinder<UGameplayEffect> DefaultForceMoveSpeedZeroEffectClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Effects/Boss/GE_ForceMoveSpeedZero");
	if (DefaultForceMoveSpeedZeroEffectClass.Succeeded()) ForceMoveSpeedZeroEffectClass = DefaultForceMoveSpeedZeroEffectClass.Class;

	StateMachineComp = CreateDefaultSubobject<UBossStateMachine_BattleCruiser>(TEXT("StateMachine"));
	AddOwnedComponent(StateMachineComp);

	LandTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("GroggyTimeline"));
	LandTimelineUpdateEvent.BindUFunction(this, TEXT("OnLandTimelineUpdate"));
	LandTimelineFinishedEvent.BindUFunction(this, TEXT("OnLandEnded"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultGroggyCurveClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Enemy_Boss/BattleCruiser/CurveFloat_Boss_BattleCruiser_Groggy");
	if (DefaultGroggyCurveClass.Succeeded()) LandCurveFloat = DefaultGroggyCurveClass.Object;
}



void AEnemy_Boss_BattleCruiser::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(LandTimelineComp))
	{
		LandTimelineComp->AddInterpFloat(LandCurveFloat, LandTimelineUpdateEvent);
		LandTimelineComp->SetTimelineFinishedFunc(LandTimelineFinishedEvent);
	}

	StateMachineComp->SetupOwnerBoss(this);
	StateMachineComp->ChangeStateByEnum(CurrentStateEnum, EBattleCruiserState::IDLE);

	OnPhase2EnteredDelegate.AddUniqueDynamic(this, &AEnemy_Boss_BattleCruiser::ForceEndGroggy);
}



void AEnemy_Boss_BattleCruiser::ApplyForceMoveSpeedZeroEffect()
{
	FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(ForceMoveSpeedZeroEffectClass,
		1.0f, AbilitySystemComponent->MakeEffectContext());
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}



void AEnemy_Boss_BattleCruiser::RemoveForceMoveSpeedZeroEffect()
{
	AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer { FGameplayTag::RequestGameplayTag(TEXT("Effect.Boss.ForceMoveSpeedZero")) });
}



void AEnemy_Boss_BattleCruiser::StartLand(bool bReverse)
{
	bReverse ? LandTimelineComp->Reverse() : LandTimelineComp->Play();
}



void AEnemy_Boss_BattleCruiser::StartGroggy()
{
	OnGroggyStart.Broadcast();
	OnLandComplete.AddUniqueDynamic(this, &AEnemy_Boss_BattleCruiser::OnGroggyStarted);

	StartLand(false);
}



void AEnemy_Boss_BattleCruiser::DieAction()
{
	ForceEndGroggy();

	CancelCancelableAbilities();

	StateMachineComp->ChangeStateByEnum(CurrentStateEnum, EBattleCruiserState::DEAD);
	
	GetWorldTimerManager().ClearAllTimersForObject(this);
	LandTimelineComp->Stop();

	Super::DieAction();
}



void AEnemy_Boss_BattleCruiser::OnLandTimelineUpdate(float Value)
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(400.0f, 30.0f, Value));
}



void AEnemy_Boss_BattleCruiser::OnLandEnded()
{
	OnLandComplete.Broadcast();
}



void AEnemy_Boss_BattleCruiser::OnGroggyStarted()
{
	OnLandComplete.RemoveAll(this);
	OnLandComplete.RemoveDynamic(this, &AEnemy_Boss_BattleCruiser::OnGroggyStarted);
	
	GetWorldTimerManager().SetTimer(GroggyTimerHandle,
		FTimerDelegate::CreateLambda([&]() -> void
		{
			if (IsValid(LandTimelineComp))
			{
				OnLandComplete.AddUniqueDynamic(this, &AEnemy_Boss_BattleCruiser::OnGroggyEnded);
				LandTimelineComp->ReverseFromEnd();
			}
		}), GroggyDuration, false);
}



void AEnemy_Boss_BattleCruiser::OnGroggyEnded()
{
	OnGroggyComplete.Broadcast();
	OnLandComplete.RemoveDynamic(this, &AEnemy_Boss_BattleCruiser::OnGroggyEnded);

	StateMachineComp->ChangeStateByEnum(CurrentStateEnum, EBattleCruiserState::IDLE);
}



void AEnemy_Boss_BattleCruiser::OnGuardChanged_Implementation(float DeltaGuard, const FGameplayTagContainer& SourceTags)
{
	UUserWidget* HealthbarWidget = GetHealthbarWidgetObject();

	if (HealthbarWidget != nullptr && HealthbarWidget->Implements<UBossHealthbarWidgetHandler>())
	{
		IBossHealthbarWidgetHandler::Execute_SetGuardPercent(HealthbarWidget, FMath::Max(0.0f, GetGuard() / DefaultGuard));
	}

	// 가드가 0 이하인 경우 그로기 상태로 진입
	if (GetGuard() <= 0.0f && CurrentStateEnum != EBattleCruiserState::GROGGY)
	{
		StateMachineComp->ChangeStateByEnum(CurrentStateEnum, EBattleCruiserState::GROGGY);
	}
}



void AEnemy_Boss_BattleCruiser::ForceEndGroggy()
{
	if (CurrentStateEnum != EBattleCruiserState::GROGGY) return;

	OnGroggyComplete.Broadcast();
	OnLandComplete.RemoveDynamic(this, &AEnemy_Boss_BattleCruiser::OnGroggyEnded);
	OnLandComplete.RemoveDynamic(this, &AEnemy_Boss_BattleCruiser::OnGroggyStarted);
	OnLandComplete.AddUniqueDynamic(this, &AEnemy_Boss_BattleCruiser::ChangeStateToIdleOnGroggyForceEnded);

	LandTimelineComp->Stop();
	StartLand(true);
}



void AEnemy_Boss_BattleCruiser::ChangeStateToIdleOnGroggyForceEnded()
{
	OnLandComplete.RemoveDynamic(this, &AEnemy_Boss_BattleCruiser::ChangeStateToIdleOnGroggyForceEnded);
	StateMachineComp->ChangeStateByEnum(CurrentStateEnum, EBattleCruiserState::IDLE);
}
