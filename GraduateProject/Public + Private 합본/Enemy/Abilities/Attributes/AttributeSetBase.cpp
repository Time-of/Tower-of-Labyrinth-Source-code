
#include "Enemy/Abilities/Attributes/AttributeSetBase.h"

#include "CharacterBase.h"

#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"



void UAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// 어트리뷰트가 변경될 때 호출되며, 최대 체력이 변경되는 경우 체력을 함께 조정하기 위한 기능
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

		const float CurrentMaxHealth = MaxHealth.GetCurrentValue();
		const float CurrentHealth = Health.GetCurrentValue();

		if (ASC != nullptr)
		{
			float NewHealth;

			// 최대 체력이 전보다 줄었을 경우
			if (CurrentMaxHealth > NewValue)
			{
				NewHealth = CurrentHealth > NewValue ? NewValue : CurrentHealth;
			}
			// 최대 체력이 전보다 늘었을 경우
			else
			{
				NewHealth = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, CurrentMaxHealth),
					FVector2D(0.0f, NewValue), CurrentHealth);
			}

			ASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewHealth);
		}
	}
}



void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float DeltaValue = 0.0f;

	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive)
	{
		DeltaValue = Data.EvaluatedData.Magnitude;
	}
	else if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Override)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* SourceASC = EffectContext.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ACharacterBase* TargetCharacter = nullptr;

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ACharacterBase>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		AActor* SourceActor = nullptr;
		AController* SourceController = nullptr;
		ACharacterBase* SourceCharacter = nullptr;

		if (SourceASC != nullptr && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = SourceASC->AbilityActorInfo->AvatarActor.Get();
			SourceController = SourceASC->AbilityActorInfo->PlayerController.Get();

			// 컨트롤러를 찾지 못할 때
			if (SourceActor != nullptr && !SourceController)
			{
				if (APawn* SourcePawn = Cast<APawn>(SourceActor))
				{
					SourceController = SourcePawn->GetController();
				}
			}

			SourceCharacter = SourceController != nullptr ? Cast<ACharacterBase>(SourceController->GetPawn()) :
				Cast<ACharacterBase>(SourceActor);

			// 이펙트 발생자가 존재한다면 소스액터를 이펙트 발생자로 설정
			SourceActor = EffectContext.GetEffectCauser() != nullptr ? EffectContext.GetEffectCauser() : SourceActor;
		}

		// HitResult 추출
		FHitResult HitResult;

		if (EffectContext.GetHitResult()) HitResult = *EffectContext.GetHitResult();

		// 적용된 피해량을 가져온 후 피해량 어트리뷰트를 0으로 초기화
		const float DamageApplied = GetDamage();

		SetDamage(0.0f);

		if (DamageApplied > 0.0f)
		{
			const float OldHealth = GetHealth();

			SetHealth(FMath::Clamp(OldHealth - DamageApplied, 0.0f, GetMaxHealth()));

			if (TargetCharacter != nullptr)
			{
				TargetCharacter->HandleDamage(DamageApplied, HitResult, SourceTags, SourceCharacter, SourceActor);
				TargetCharacter->OnHealthChanged(-DamageApplied, SourceTags);
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetDamageToDirectHealthAttribute())
	{
		// 적용된 피해량을 가져온 후 피해량 어트리뷰트를 0으로 초기화
		const float DamageApplied = GetDamageToDirectHealth();

		SetDamageToDirectHealth(0.0f);

		if (DamageApplied > 0.0f && TargetCharacter != nullptr)
		{
			const float OldHealth = GetHealth();

			SetHealth(FMath::Clamp(OldHealth - DamageApplied, 0.0f, GetMaxHealth()));

			TargetCharacter->HandleHealthChanged(-DamageApplied, SourceTags);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (TargetCharacter != nullptr)
		{
			SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

			TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetGuardAttribute())
	{
		if (TargetCharacter != nullptr)
		{
			SetGuard(FMath::Clamp(GetGuard(), 0.0f, GetInternalMaxGuard()));

			TargetCharacter->OnGuardChanged(DeltaValue, SourceTags);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetStrengthAttribute())
	{
		if (TargetCharacter != nullptr)
		{
			TargetCharacter->OnStrengthChanged(DeltaValue, SourceTags);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetDefenseAttribute())
	{
		if (TargetCharacter != nullptr)
		{
			TargetCharacter->OnDefenseChanged(DeltaValue, SourceTags);
		}
	}
}
