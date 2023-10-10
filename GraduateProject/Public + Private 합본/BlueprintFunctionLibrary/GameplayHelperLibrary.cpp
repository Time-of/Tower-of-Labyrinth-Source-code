
#include "BlueprintFunctionLibrary/GameplayHelperLibrary.h"

#include "BlueprintFunctionLibrary/DataSaveLoadHelper.h"
#include "GameInstance/GameInstanceBase.h"
#include "GameInstance/GISS_ShuffleLevel.h"
#include "Player/PlayerCharacterBase.h"

#include "GameFramework/GameModeBase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



void UGameplayHelperLibrary::ApplySimpleKnockbackToPC(AActor* FromActor, float KnockbackPower, float KnockbackAirRate)
{
	if (!IsValid(FromActor)) return;

	ACharacter* PC = UGameplayStatics::GetPlayerCharacter(FromActor, 0);
	
	FVector Direction = (PC->GetActorLocation() - FromActor->GetActorLocation());
	Direction.Z = 0;
	Direction = Direction.GetSafeNormal();
	PC->LaunchCharacter(Direction * KnockbackPower + FVector::UpVector * KnockbackPower * KnockbackAirRate,
		false, false);
	//PC->GetCharacterMovement()->ApplyAccumulatedForces(FromActor->GetWorld()->GetDeltaSeconds());
	//PC->GetCharacterMovement()->HandlePendingLaunch();
	//PC->GetCharacterMovement()->ClearAccumulatedForces();


	// 23-05-05: 만약 넉백이 의도대로 동작하지 않는다면, 루트모션이 적용중이기 때문이다.

	//UE_LOG(LogTemp, Warning, TEXT("넉백!! %s"), *(KnockbackPower * Direction).ToString());
}



void UGameplayHelperLibrary::ApplyDistanceBasedKnockbackToPC(AActor* FromActor, float KnockbackPower, float KnockbackAirRate)
{
	if (!IsValid(FromActor)) return;

	ACharacter* PC = UGameplayStatics::GetPlayerCharacter(FromActor, 0);

	FVector Direction = (PC->GetActorLocation() - FromActor->GetActorLocation());
	float LocationDist = Direction.Size();
	Direction /= (LocationDist != 0.0f ? LocationDist : 0.000001f);

	float CalculatedKnockbackPower = (500.0f / (500.0f + LocationDist)) * KnockbackPower;

	PC->LaunchCharacter(Direction * CalculatedKnockbackPower + FVector::UpVector * CalculatedKnockbackPower * KnockbackAirRate,
		false, false);
}



void UGameplayHelperLibrary::SpawnHitEffects(AActor* WorldContext, AActor* TargetActor, USoundBase* HitSfx, UNiagaraSystem* HitVfx, FVector VfxScaleVector)
{
	// 사운드, 이펙트는 각각의 스폰 함수 내에 null체크가 이미 존재하기에 수행 X
	if (!IsValid(WorldContext) || !IsValid(TargetActor)) return;

	UGameplayStatics::SpawnSoundAtLocation(WorldContext,
		HitSfx,
		TargetActor->GetActorLocation());

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldContext,
		HitVfx,
		UKismetMathLibrary::RandomPointInBoundingBox(TargetActor->GetActorLocation(), FVector::OneVector * 10.0f),
		UKismetMathLibrary::RandomRotator(true),
		VfxScaleVector,
		true,
		true,
		ENCPoolMethod::AutoRelease);
}



void UGameplayHelperLibrary::SpawnHitEffectsParticle(AActor* WorldContext, AActor* TargetActor, USoundBase* HitSfx, UParticleSystem* HitVfx, FVector VfxScaleVector)
{
	// 사운드, 이펙트는 각각의 스폰 함수 내에 null체크가 이미 존재하기에 수행 X
	if (!IsValid(WorldContext) || !IsValid(TargetActor)) return;

	UGameplayStatics::SpawnSoundAtLocation(WorldContext,
		HitSfx,
		TargetActor->GetActorLocation());

	UGameplayStatics::SpawnEmitterAtLocation(WorldContext,
		HitVfx,
		UKismetMathLibrary::RandomPointInBoundingBox(TargetActor->GetActorLocation(), FVector::OneVector * 10.0f),
		UKismetMathLibrary::RandomRotator(true),
		VfxScaleVector,
		true,
		EPSCPoolMethod::AutoRelease);
}



void UGameplayHelperLibrary::OpenNextLevel(AActor* WorldContext)
{
	if (!IsValid(WorldContext)) return;

	UGameInstanceBase* GIBase = WorldContext->GetGameInstance<UGameInstanceBase>();

	UGISS_ShuffleLevel* ShuffleSS = GIBase->GetSubsystem<UGISS_ShuffleLevel>();

	FName LevelNameToOpen = ShuffleSS->GetLevelNameByIndex(ShuffleSS->CurrentLevelCount++);

	if (LevelNameToOpen == "LEVEL_END")
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("레벨 오픈: 더 이상 오픈할 레벨이 없습니다!!"));
#endif
		return;
	}

#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("레벨 오픈: %d 인덱스의 %s 레벨을 오픈 중..."), ShuffleSS->CurrentLevelCount - 1, *LevelNameToOpen.ToString());
#endif

	

	UDataSaveLoadHelper::OpenLevelWithPlayerData(WorldContext, LevelNameToOpen);

	GIBase->ShowLoadingScreen();
}



APlayerCharacterBase* UGameplayHelperLibrary::GetPlayerCharacter(AActor* WorldContext)
{
	return UGameplayStatics::GetPlayerController(WorldContext, 0)->GetPawn<APlayerCharacterBase>();
}
