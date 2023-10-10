
#include "Enemy/Animation/NS_EnemyAttack.h"

#include "Enemy/EnemyCharacterBase.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Interface/EnemyHandler.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/CurveTable.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"



UNS_EnemyAttack::UNS_EnemyAttack() : Super(),
	bUseStrengthToDamage(false),
	DamageMultRead(0.0f),
	TimeElapsed(0.0f),
	TickDelay(0.05f), SphereRadius(50.0f), SocketName(TEXT("None")),
	bDrawDebug(false)
{
	//NotifyColor = FColor(255, 100, 30, 255);
}



void UNS_EnemyAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!GWorld->HasBegunPlay()) return;

	TimeElapsed = TickDelay;

	IgnoreActors = { MeshComp->GetOwner() };

	PrevLocation = MeshComp->GetSocketLocation(SocketName);

	if (AEnemyCharacterBase* OwnerEnemy = Cast<AEnemyCharacterBase>(MeshComp->GetOwner()))
	{
		if (!bUseStrengthToDamage)
		{
			// 커브 테이블에서 DamageMultRead에 피해량 정보 읽어오기
			if (!DamageCurveTable.Eval(static_cast<float>(OwnerEnemy->GetCharacterLevel()),
				&DamageMultRead, FString("")))
			{
				UE_LOG(LogTemp, Warning, TEXT("NS_EnemyAttack: %s 애니메이션에서 커브 테이블 행 %s 읽어올 수 없음!!"),
					*Animation->GetName(), *DamageCurveTable.RowName.ToString());

				DamageMultRead = 0.0f;
			}
		}
		else
		{
			DamageMultRead = OwnerEnemy->GetStrength();
		}
	}

	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}



void UNS_EnemyAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	TimeElapsed += FrameDeltaTime;

	if (TimeElapsed < TickDelay || !GWorld->HasBegunPlay() || !IsValid(MeshComp->GetOwner())) return;

	TimeElapsed -= TickDelay;

	TryTrace(MeshComp);

	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}



void UNS_EnemyAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!GWorld->HasBegunPlay()) return;

	TryTrace(MeshComp);

	Super::NotifyEnd(MeshComp, Animation, EventReference);
}



void UNS_EnemyAttack::TryTrace(const USkeletalMeshComponent* MeshComp)
{
	const FVector Location = MeshComp->GetSocketLocation(SocketName);

	TArray<FHitResult> HitResultArray;

	bool bHit = UKismetSystemLibrary::SphereTraceMulti(MeshComp->GetWorld(), PrevLocation, Location, SphereRadius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false, IgnoreActors, bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, HitResultArray, true);

	for (const auto& HitResult : HitResultArray)
	{
		if (HitResult.GetActor()->ActorHasTag(TEXT("Friendly")))
		{
			FGameplayEventData EventData;

			UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
			if (!IsValid(ASC))
			{
			#if WITH_EDITOR
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("NS_EnemyAttack: 공격하고자 하는 소스 액터의 어빌리티 시스템 없음!!"));
			#endif

				break;
			}
			
			EventData.ContextHandle = ASC->MakeEffectContext();
			EventData.Instigator = MeshComp->GetOwner();
			EventData.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);
			EventData.EventMagnitude = DamageMultRead;
			
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(),
				FGameplayTag::RequestGameplayTag(TEXT("Event.Enemy.Ability.Hit")),
				EventData);
		}

		IgnoreActors.AddUnique(HitResult.GetActor());
	}

	PrevLocation = Location;
}
