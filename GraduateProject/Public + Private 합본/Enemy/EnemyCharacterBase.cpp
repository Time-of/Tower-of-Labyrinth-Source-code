
#include "Enemy/EnemyCharacterBase.h"

#include "GameAssetManager.h"
#include "Enemy/EnemyAIControllerBase.h"
#include "Enemy/AI/BBKeys.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Abilities/GameplayAbilityBase.h"
#include "Enemy/Abilities/Attributes/AttributeSetBase.h"
#include "Enemy/DataTable/EnemyDropTableRow.h"
#include "GameInstance/GameInstanceBase.h"
#include "GameInstance/GISS_PlayerMoney.h"
#include "GameInstance/GISS_ShuffleLevel.h"
#include "Item/ItemBase.h"
#include "Item/ItemPickupBase.h"
#include "Item/MoneyPickupBase.h"
#include "Widget/Interface/FloatingWidgetHandler.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"



AEnemyCharacterBase::AEnemyCharacterBase() : Super(),
	bHaveToDoPatrol(true),
	bForcePerceptionOnHit(true),
	bPerceptionedByHit(false),
	InternalEnemyName(TEXT("DefaultEnemyName")), DisplayEnemyName(TEXT("DefaultEnemyActualName")),
	MaxMoneyAmountPerPickup(500.0f),
	StiffLight_LocalDirection(FVector(-35.0f, 0.0f, 50.0f)),
	DefaultGuard(0.0f), bShowHealthbarWidgetOnHealthChanged(true),
	bHideHealthbarOnHitAfterDelay(true),
	HideHealthbarTimeline(nullptr), HideHealthbarTimelineAnimCurveFloat(nullptr),
	DeathSfx(nullptr),
	DeathFadeoutTimeline(nullptr), DeathFadeoutAnimCurveFloat(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 420.0f, 0.0f);

	HealthbarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Healthbar Widget Comp"));
	HealthbarWidgetComp->SetupAttachment(GetCapsuleComponent());
	HealthbarWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	HealthbarWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	HealthbarWidgetComp->SetDrawSize(FVector2D(200.0f, 16.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultHealthbarClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Widget/WBP_FloatingHealthbar");

	if (DefaultHealthbarClass.Succeeded())
	{
		HealthbarWidgetComp->SetWidgetClass(DefaultHealthbarClass.Class);
	}

	static ConstructorHelpers::FClassFinder<UWidgetComponent> DefaultDamageTextClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Widget/WC_FloatingDamageWidgetComp");

	if (DefaultDamageTextClass.Succeeded())
	{
		DamageTextWidgetComponentClass = DefaultDamageTextClass.Class;
	}

	HideHealthbarTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("HideHealthbarTimelineComp"));
	HideHealthbarTimelineUpdate.BindUFunction(this, TEXT("OnHideHealthbarTimelineUpdate"));
	HideHealthbarTimelineFinished.BindUFunction(this, TEXT("OnHideHealthbarTimelineFinished"));
	
	DeathFadeoutTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DeathFadeoutTimelineComp"));
	DeathFadeoutAnimTimelineUpdate.BindUFunction(this, TEXT("OnDeathFadeoutTimelineUpdate"));
	DeathFadeoutAnimTimelineFinished.BindUFunction(this, TEXT("OnDeathFadeoutTimelineFinished"));

	AIControllerClass = AEnemyAIControllerBase::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Tags = TArray<FName>{ TEXT("Hostile") };

	static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultDeathFadeoutCurveFloat = TEXT("/Game/SungSoo_Workspace/DataTables/CurveFloat_EnemyDeathFadeout");

	if (DefaultDeathFadeoutCurveFloat.Succeeded())
	{
		DeathFadeoutAnimCurveFloat = DefaultDeathFadeoutCurveFloat.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultHideHealthbarCurveFloat = TEXT("/Game/SungSoo_Workspace/DataTables/CurveFloat_EnemyHideHealthbar");

	if (DefaultHideHealthbarCurveFloat.Succeeded())
	{
		HideHealthbarTimelineAnimCurveFloat = DefaultHideHealthbarCurveFloat.Object;
	}
}



void AEnemyCharacterBase::Stiff_Light()
{
	LaunchCharacter(GetActorRotation().RotateVector(StiffLight_LocalDirection), false, false);
}



void AEnemyCharacterBase::SetGuardToFull_Implementation()
{
	SetGuardGuage(DefaultGuard);
}



TSubclassOf<UGameplayEffect> AEnemyCharacterBase::GetAdditionalAttackEffectOnTarget_Implementation() const
{
	return AdditionalAttackEffectOnTarget;
}



void AEnemyCharacterBase::OnHealthChanged_Implementation(float DeltaHealth, const FGameplayTagContainer& SourceTags)
{
	if (DeltaHealth < 0.0f && GetHealth() > 0.0f)
	{
		ShowDamageText(DeltaHealth);
	}
	
	if (IsAlive())
	{
		Stiff_Light();

		if (bShowHealthbarWidgetOnHealthChanged) UpdateHealthbar(true);
	}
	else
	{
		HandleDie();
	}
}



void AEnemyCharacterBase::OnHideHealthbarTimelineUpdate(float Value)
{
}



void AEnemyCharacterBase::OnHideHealthbarTimelineFinished()
{
	if (!bHideHealthbarOnHitAfterDelay) return;

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("체력바 없어짐!!"));

	if (UUserWidget* HealthbarWidget = HealthbarWidgetComp->GetUserWidgetObject())
	{
		if (HealthbarWidget->Implements<UFloatingWidgetHandler>())
			IFloatingWidgetHandler::Execute_HideWidget(HealthbarWidget);
	}
}



void AEnemyCharacterBase::OnDeathFadeoutTimelineUpdate_Implementation(float Value)
{
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("AppearPercent"), Value);
}



void AEnemyCharacterBase::OnDeathFadeoutTimelineFinished()
{
	Destroy();
}



void AEnemyCharacterBase::HandleCancelCancelableAbilities()
{
	CancelCancelableAbilities();
}



void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	DefaultGuard = GetGuard();

	if (IsValid(DeathFadeoutTimeline))
	{
		DeathFadeoutTimeline->AddInterpFloat(DeathFadeoutAnimCurveFloat, DeathFadeoutAnimTimelineUpdate);
		DeathFadeoutTimeline->SetTimelineFinishedFunc(DeathFadeoutAnimTimelineFinished);
	}

	if (IsValid(HideHealthbarTimeline))
	{
		HideHealthbarTimeline->AddInterpFloat(HideHealthbarTimelineAnimCurveFloat, HideHealthbarTimelineUpdate);
		HideHealthbarTimeline->SetTimelineFinishedFunc(HideHealthbarTimelineFinished);
	}

	if (AAIController* AIC = GetController<AAIController>())
	{
		AIC->GetBlackboardComponent()->SetValueAsBool(BBKeys::bHaveToDoPatrol, bHaveToDoPatrol);
	}
}



void AEnemyCharacterBase::DieAction()
{
	Super::DieAction();

	UpdateHealthbar(false);

	CustomTimeDilation = 1.0f;

	OnEnemyDied.Broadcast(this);

	if (IsValid(DeathSfx))
		UGameplayStatics::SpawnSoundAtLocation(this, DeathSfx, GetActorLocation(), GetActorRotation());

	DropItems();

	if (IsValid(DeathFadeoutTimeline))
	{
		DeathFadeoutTimeline->PlayFromStart();
	}
	else
	{
		OnDeathFadeoutTimelineFinished();
	}

	if (APlayerController* FirstPC = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* PlayerCharacter = FirstPC->GetPawn())
		{
			FGameplayEventData EventData;

			EventData.ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
			EventData.Instigator = this;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerCharacter,
				FGameplayTag::RequestGameplayTag(TEXT("Event.Enemy.Died")),
				EventData);
		}
	}
}



void AEnemyCharacterBase::InitializeDefaultAttribute()
{
	if (bDefaultAttributeInitialized)
	{
		GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
		return;
	}

	const UAttributeSetBase* AttributeSet = AbilitySystemComponent->GetSet<UAttributeSetBase>();



	UGISS_ShuffleLevel* ShuffleLvSS = GetGameInstance()->GetSubsystem<UGISS_ShuffleLevel>();

	AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetLevelAttribute(),
		EGameplayModOp::Override, FMath::Max(ShuffleLvSS->CurrentLevelCount / 6 + 1, 0));



	UGameInstanceBase* GIBase = GetGameInstance<UGameInstanceBase>();

	FStatAttributeInfo FoundStatInfo = GIBase->GetStatByLevel(InternalEnemyName, GetCharacterLevel());
	

	AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetMaxHealthAttribute(),
		EGameplayModOp::Override, FoundStatInfo.MaxHealth);

	AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetHealthAttribute(),
		EGameplayModOp::Override, FoundStatInfo.MaxHealth);

	AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetStrengthAttribute(),
		EGameplayModOp::Override, FoundStatInfo.Strength);

	AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetDefenseAttribute(),
		EGameplayModOp::Override, FoundStatInfo.Defense);

	AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetAttackSpeedAttribute(),
		EGameplayModOp::Override, FoundStatInfo.AttackSpeed);

	AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetMoveSpeedAttribute(),
		EGameplayModOp::Override, FoundStatInfo.MoveSpeed);

	AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetGuardAttribute(),
		EGameplayModOp::Override, FoundStatInfo.Guard);

	AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetGuardDamageTakenMultAttribute(),
		EGameplayModOp::Override, FoundStatInfo.GuardDamageTakenMult);

	AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetDamageMultOnGuardExistsAttribute(),
		EGameplayModOp::Override, FoundStatInfo.DamageTakenMultOnGuardExists);



	AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetInternalMaxGuardAttribute(),
		EGameplayModOp::Override, FoundStatInfo.Guard);

	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();

	bDefaultAttributeInitialized = true;

#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("%s 몬스터의 레벨: %d, 체력: %f"), *GetName(), GetCharacterLevel(), GetHealth());
#endif
}



void AEnemyCharacterBase::OnDamaged_Implementation(float DamageAmount, const FHitResult& HitResult, const FGameplayTagContainer& DamageTags, ACharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	if (!bPerceptionedByHit && bForcePerceptionOnHit)
	{
		bPerceptionedByHit = true;
		
		if (AEnemyAIControllerBase* AIC = GetController<AEnemyAIControllerBase>())
		{
			ACharacter* PC = UGameplayStatics::GetPlayerCharacter(this, 0);
			AIC->GetBlackboardComponent()->SetValueAsObject(BBKeys::TargetActor, PC);
			AIC->GetBlackboardComponent()->SetValueAsBool(BBKeys::bHasLineOfSight, true);
			AIC->GetBlackboardComponent()->SetValueAsFloat(BBKeys::DistanceFromTarget,
				(PC->GetActorLocation() - GetActorLocation()).Size());
			//AIC->SetFocus(PC, EAIFocusPriority::Default);
		}
	}
}



void AEnemyCharacterBase::CancelCancelableAbilities()
{
	FGameplayTagContainer AbilityTagToCancel{ FGameplayTag::RequestGameplayTag(TEXT("Ability.Enemy.CanBeCanceled")) };

	AbilitySystemComponent->CancelAbilities(&AbilityTagToCancel);
}



void AEnemyCharacterBase::DropItems()
{
	UGameInstanceBase* GIBase = GetGameInstance<UGameInstanceBase>();

	if (FEnemyDropTableRow* DropTableRow = GIBase->GetEnemyDropTableRow(InternalEnemyName))
	{
		bool bDropMoney = UKismetMathLibrary::RandomFloat() <= DropTableRow->MoneyDropChance;

		if (!bDropMoney) return;

		// 드랍해야 할 남은 돈
		float RemainMoneyDrop = DropTableRow->MoneyDropAmount * 
			FMath::Max(GetCharacterLevel(), 1.0f);

		UE_LOG(LogTemp, Warning, TEXT("몬스터 %s가 드랍해야 하는 돈: %s"), *FString(GetName()), *FString::SanitizeFloat(RemainMoneyDrop));

		UGISS_PlayerMoney* MoneySS = GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>();

		// 픽업 돈 하나 당 최대 돈을 설정하여 돈이 많이 나오면 시각적으로 알 수 있게 함
		while (RemainMoneyDrop > 0.0f)
		{
			FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), FVector(30.0f));

			FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);

			if (AMoneyPickupBase* MoneyPickup = GetWorld()->SpawnActorDeferred<AMoneyPickupBase>(MoneySS->MoneyPickupClass, SpawnTransform, (AActor*)nullptr, (APawn*)nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn))
			{
				// 최대 돈 수량 조정...
				MoneyPickup->SetMoneyAmount(FMath::Min(RemainMoneyDrop, MaxMoneyAmountPerPickup));
				UGameplayStatics::FinishSpawningActor(MoneyPickup, SpawnTransform);
			}

			RemainMoneyDrop -= MaxMoneyAmountPerPickup;
		}

		UE_LOG(LogTemp, Warning, TEXT("몬스터 %s가 드랍하고 남은 돈: %s"), *FString(GetName()), *FString::SanitizeFloat(RemainMoneyDrop));

		UGameAssetManager& AssetManager = UGameAssetManager::GetInstance();

		for (const FDropItemInfo& DropInfo : DropTableRow->DropItems)
		{
			if (UKismetMathLibrary::RandomFloat() <= DropInfo.DropChance)
			{
				FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), FVector(35.0f));

				FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);

				AItemPickupBase* PickupItem = GetWorld()->SpawnActorDeferred<AItemPickupBase>(GIBase->GetItemPickupClass(),
					SpawnTransform, (AActor*)nullptr, (APawn*)nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
				UItemBase* LoadedItem = AssetManager.LoadItem(DropInfo.DropItem);

				if (IsValid(PickupItem) && IsValid(LoadedItem))
				{
					PickupItem->SetPickupItemInfo(LoadedItem, DropInfo.DropAmount);
				}

				UGameplayStatics::FinishSpawningActor(PickupItem, SpawnTransform);
			}
		}
	}
}



void AEnemyCharacterBase::UpdateHealthbar_Implementation(bool bShowHealthbar)
{
	if (UUserWidget* HealthbarWidget = HealthbarWidgetComp->GetUserWidgetObject())
	{
		if (bShowHealthbar)
		{
			if (HealthbarWidget->GetVisibility() == ESlateVisibility::Hidden || HealthbarWidget->GetVisibility() == ESlateVisibility::Collapsed)
				HealthbarWidget->SetVisibility(ESlateVisibility::Visible);

			if (HealthbarWidget->Implements<UFloatingWidgetHandler>())
				IFloatingWidgetHandler::Execute_SetProgressPercent(HealthbarWidget, GetHealth() / GetMaxHealth());
			
			if (bHideHealthbarOnHitAfterDelay)
			{
				HideHealthbarTimeline->PlayFromStart();
			}
		}
		else
		{
			HealthbarWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}



void AEnemyCharacterBase::ShowDamageText_Implementation(float DamageAmount)
{
	if (!IsValid(DamageTextWidgetComponentClass)) return;

	UActorComponent* DamageTextComp = AddComponentByClass(DamageTextWidgetComponentClass, true, GetActorTransform(), false);

	if (DamageTextComp->Implements<UFloatingWidgetHandler>())
	{
		IFloatingWidgetHandler::Execute_SetWidgetText(DamageTextComp, FText::AsNumber(FMath::RoundToInt(DamageAmount)));
	}
}