
#include "Item/MoneyPickupBase.h"

#include "GameInstance/GISS_PlayerMoney.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



AMoneyPickupBase::AMoneyPickupBase() : Super(),
	MinWaitDuration(0.6f), MaxWaitDuration(1.2f),
	StartLocation(FVector::ZeroVector), MoneyAmount(0.0f)
{
	PrimaryActorTick.bCanEverTick = false;

	PickupAnimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PickupAnimTimelineComp"));
	PickupAnimTimelineUpdate.BindUFunction(this, TEXT("OnPickupAnimTimelineUpdate"));
	PickupAnimTimelineFinished.BindUFunction(this, TEXT("OnPickupAnimTimelineFinished"));

	CollisionSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComp->SetCollisionProfileName(TEXT("PhysicsActor"));
	CollisionSphereComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CollisionSphereComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	CollisionSphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CollisionSphereComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	CollisionSphereComp->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	CollisionSphereComp->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
	CollisionSphereComp->SetCollisionResponseToChannel(UEngineTypes::ConvertToCollisionChannel(ETraceTypeQuery::TraceTypeQuery2), ECR_Ignore);
	CollisionSphereComp->SetCollisionResponseToChannel(UEngineTypes::ConvertToCollisionChannel(ETraceTypeQuery::TraceTypeQuery3), ECR_Ignore);
	CollisionSphereComp->SetCollisionResponseToChannel(UEngineTypes::ConvertToCollisionChannel(ETraceTypeQuery::TraceTypeQuery4), ECR_Ignore);
	CollisionSphereComp->SetComponentTickEnabled(false);
	CollisionSphereComp->SetSimulatePhysics(true);
	
	// SetMassOverrideInKg()는 빌드 실패의 주범....
	//CollisionSphereComp->SetMassOverrideInKg(NAME_None, 3.0f);
	CollisionSphereComp->BodyInstance.bOverrideMass = true;
	CollisionSphereComp->BodyInstance.SetMassOverride(3.0f);

	CollisionSphereComp->SetSphereRadius(25.0f);
	//CollisionSphereComp->SetupAttachment(GetRootComponent());
	SetRootComponent(CollisionSphereComp);

	MoneyStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MoneyStaticMesh"));
	MoneyStaticMeshComp->SetComponentTickEnabled(false);
	MoneyStaticMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	MoneyStaticMeshComp->SetupAttachment(CollisionSphereComp);
}



void AMoneyPickupBase::SetMoneyAmount(float NewAmount)
{
	MoneyAmount = NewAmount;
}



void AMoneyPickupBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(PickupAnimTimeline))
	{
		PickupAnimTimeline->AddInterpFloat(PickupAnimCurveFloat, PickupAnimTimelineUpdate);
		PickupAnimTimeline->SetTimelineFinishedFunc(PickupAnimTimelineFinished);
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
		MoneyVFX, GetActorLocation(), FRotator::ZeroRotator, FVector::OneVector, true, true, ENCPoolMethod::AutoRelease);

	// BP에서 StartPickupAnim() 호출 필수!!



	//FTimerHandle InitialDelayTimerHandle;
	
	//GetWorldTimerManager().SetTimer(InitialDelayTimerHandle, FTimerDelegate::CreateLambda([&]()
		//{
			
	//	}), UKismetMathLibrary::RandomFloatInRange(MinWaitDuration, MaxWaitDuration), false);
}



void AMoneyPickupBase::OnPickupAnimTimelineUpdate(float Value)
{
	if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		SetActorLocation(UKismetMathLibrary::VLerp(StartLocation, Character->GetActorLocation(), Value));
	}
}



void AMoneyPickupBase::OnPickupAnimTimelineFinished()
{
	if (UGISS_PlayerMoney* MoneySS = GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>())
	{
		MoneySS->AddMoney(MoneyAmount);
	}

	Destroy();
}

void AMoneyPickupBase::StartPickupAnim()
{
	CollisionSphereComp->SetSimulatePhysics(false);
	CollisionSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StartLocation = GetActorLocation();

	PickupAnimTimeline->PlayFromStart();
}
