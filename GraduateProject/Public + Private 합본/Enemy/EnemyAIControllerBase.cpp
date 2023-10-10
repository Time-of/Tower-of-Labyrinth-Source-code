
#include "Enemy/EnemyAIControllerBase.h"

#include "Enemy/EnemyCharacterBase.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/AI/BBKeys.h"
#include "Player/PlayerCharacterBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Touch.h"



AEnemyAIControllerBase::AEnemyAIControllerBase() : Super(),
	bIsPlayerDied(false),
	SightSense(nullptr), TouchSense(nullptr),
	BTAsset(nullptr), BBAsset(nullptr),
	MontageOnPlayerDied(nullptr)
{
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception")));

	SightSense = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightSenseConfig"));
	SightSense->SightRadius = 1000.0f;
	SightSense->LoseSightRadius = 1750.0f;
	SightSense->PeripheralVisionAngleDegrees = 100.0f;
	SightSense->SetMaxAge(10.0f);

	SightSense->DetectionByAffiliation.bDetectEnemies = true;
	SightSense->DetectionByAffiliation.bDetectFriendlies = false;
	SightSense->DetectionByAffiliation.bDetectNeutrals = false;

	GetPerceptionComponent()->SetDominantSense(*SightSense->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIControllerBase::OnPerceptionUpdated);

	TouchSense = CreateDefaultSubobject<UAISenseConfig_Touch>(TEXT("TouchSenseConfig"));
	TouchSense->SetMaxAge(3.0f);

	GetPerceptionComponent()->ConfigureSense(*SightSense);
	GetPerceptionComponent()->ConfigureSense(*TouchSense);

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/SungSoo_Workspace/Blueprints/AI/BB_Enemy.BB_Enemy'"));

	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	SetGenericTeamId(FGenericTeamId(1));
}



UAbilitySystemComponent* AEnemyAIControllerBase::GetAbilitySystemComponent() const
{
	AEnemyCharacterBase* EnemyCharacter = GetPawn<AEnemyCharacterBase>();

	if (EnemyCharacter == nullptr) return nullptr;

	return EnemyCharacter->GetAbilitySystemComponent();
}



UAttributeSetBase* AEnemyAIControllerBase::GetAttributeSetBase() const
{
	AEnemyCharacterBase* EnemyCharacter = GetPawn<AEnemyCharacterBase>();

	if (EnemyCharacter == nullptr) return nullptr;

	return EnemyCharacter->GetAttributeSetBase();
}



void AEnemyAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemyCharacterBase* EnemyCharacter = GetPawn<AEnemyCharacterBase>();

	if (IsValid(EnemyCharacter))
	{
		EnemyCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(this, EnemyCharacter);
	}

	RunAI();
}



ETeamAttitude::Type AEnemyAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			FGenericTeamId OtherPawnTeamId = TeamAgent->GetGenericTeamId();

			return (OtherPawnTeamId == 1) ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;
		}
	}

	return ETeamAttitude::Hostile;
}



void AEnemyAIControllerBase::OnPerceptionUpdated(AActor* Actor, const FAIStimulus Stimulus)
{
	if (bIsPlayerDied) return;
	
	Blackboard->SetValueAsObject(BBKeys::TargetActor, Actor);
	Blackboard->SetValueAsBool(BBKeys::bHasLineOfSight, Stimulus.WasSuccessfullySensed());
}



void AEnemyAIControllerBase::RunAI()
{
	UBlackboardComponent* BBComp = Blackboard;

	if (UseBlackboard(BBAsset, BBComp))
	{
		RunBehaviorTree(BTAsset);
	}

	Blackboard->SetValueAsVector(BBKeys::InitialLocation, GetPawn()->GetActorLocation());
	Blackboard->SetValueAsFloat(BBKeys::DistanceFromTarget, -1.0f);
}



void AEnemyAIControllerBase::StopAI()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);

	if (BTComp != nullptr)
	{
		BTComp->StopTree(EBTStopMode::Safe);
	}
}



void AEnemyAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (AEnemyCharacterBase* Enemy = GetPawn<AEnemyCharacterBase>())
	{
		Enemy->OnEnemyDied.AddDynamic(this, &AEnemyAIControllerBase::OnEnemyPawnDied);
	}

	if (APlayerCharacterBase* Player = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacterBase>())
	{
		Player->OnPlayerDied.AddDynamic(this, &AEnemyAIControllerBase::OnPlayerDied);
	}
}



void AEnemyAIControllerBase::OnEnemyPawnDied(AEnemyCharacterBase* Enemy)
{
	StopAI();

	Enemy->OnEnemyDied.RemoveDynamic(this, &AEnemyAIControllerBase::OnEnemyPawnDied);
}



void AEnemyAIControllerBase::OnPlayerDied(APlayerCharacterBase* Player)
{
	StopAI();

	bIsPlayerDied = true;

	Blackboard->SetValueAsBool(BBKeys::bHasLineOfSight, false);

	Blackboard->SetValueAsFloat(BBKeys::DistanceFromTarget, -1.0f);


	if (AEnemyCharacterBase* Enemy = GetPawn<AEnemyCharacterBase>())
	{
		Enemy->PlayAnimMontage(MontageOnPlayerDied);
	}
}

