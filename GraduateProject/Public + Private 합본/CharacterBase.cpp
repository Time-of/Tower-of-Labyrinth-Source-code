
#include "CharacterBase.h"

#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Abilities/GameplayAbilityBase.h"
#include "Enemy/Abilities/Attributes/AttributeSetBase.h"
#include "GameInstance/GameInstanceBase.h"
#include "GameInstance/GISS_PlayerInventory.h"
#include "Item/Item_ArmorBase.h"
#include "Item/Item_CharacterSkillBase.h"
#include "Item/Item_WeaponBase.h"
#include "Item/ItemBase.h"
#include "Player/PlayerCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"



ACharacterBase::ACharacterBase() : Super(),
	AdditionalAttackEffectOnTarget(nullptr),
	AbilitySystemComponent(nullptr),
	AttributeSetBase(nullptr),
	bExplicitlyDead(false),
	StartupEffects(),
	Abilities(),
	DeathMontage(nullptr),
	DeathMontageStartSectionName(TEXT("NAME_None")),
	EquippedWeapon(nullptr), EquippedArmor(nullptr),
	bDefaultAttributeInitialized(false),
	AIPerceptionComponent(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemCompBase>(TEXT("AbilitySystemComp"));

	AttributeSetBase = CreateDefaultSubobject<UAttributeSetBase>(TEXT("AttributeSetBase"));
	
	DeadTag = FGameplayTag::RequestGameplayTag(TEXT("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(TEXT("Effect.RemoveOnDeath"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerception"));
	AIPerceptionComponent->RegisterForSense(TSubclassOf<UAISense_Sight>());
	AIPerceptionComponent->RegisterWithPerceptionSystem();
}



// 간혹 클래스 블루프린트 복제 시 AttributeSetBase가 nullptr인 상태로 복제되어 고칠 수 없는 크래시가 발생하는 경우가 많이 있었는데, 이 기능을 추가해 픽스
void ACharacterBase::PostInitProperties()
{
	Super::PostInitProperties();

	if (!IsValid(AttributeSetBase))
		AttributeSetBase = NewObject<UAttributeSetBase>(this, (TEXT("AttributeSetBaseComp")));

	if (!IsValid(AbilitySystemComponent))
		AbilitySystemComponent = NewObject<UAbilitySystemCompBase>(this, (TEXT("EnemyAbilitySystemComp")));
}



void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(TEXT("State.Debuff.Stun")),
			EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ACharacterBase::OnStunTagChanged);

		MoveSpeedChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMoveSpeedAttribute()).
			AddUObject(this, &ACharacterBase::OnMoveSpeedAttributeChanged);
	}
}



UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}



UAttributeSetBase* ACharacterBase::GetAttributeSetBase() const
{
	return AttributeSetBase;
}



void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(NewController, this);
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
		InitializeDefaultAttribute();
		AddStartupEffects();
		AddAbilities();
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("%s 오브젝트의 %s(): AbilitySystemComponent 유효하지 않음!"), *GetName(), *FString(__FUNCTION__));
		return;
	}
}



bool ACharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}



int32 ACharacterBase::GetCharacterLevel() const
{
	const UAttributeSetBase* AttributeSet = AbilitySystemComponent->GetSet<UAttributeSetBase>();
	return AttributeSet ? AttributeSet->GetLevel() : -10.0f;
}



float ACharacterBase::GetHealth() const
{
	//return IsValid(AttributeSetBase) ? AttributeSetBase->GetHealth() : 1.0f;

	const UAttributeSetBase* AttributeSet = AbilitySystemComponent->GetSet<UAttributeSetBase>();
	return AttributeSet ? AttributeSet->GetHealth() : -10.0f;
}



float ACharacterBase::GetMaxHealth() const
{
	//return AttributeSetBase->GetMaxHealth();

	const UAttributeSetBase* AttributeSet = AbilitySystemComponent->GetSet<UAttributeSetBase>();
	return AttributeSet ? AttributeSet->GetMaxHealth() : -10.0f;
}



float ACharacterBase::GetStrength() const
{
	if (AbilitySystemComponent == nullptr || !IsValid(AbilitySystemComponent))
	{
	#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange,
			FString::Printf(
				TEXT("GetStrength(): 오류!! ASC 없음: %s"), *GetName()
				));
	#endif

		return 0.0f;
	}

	const UAttributeSetBase* AttributeSet = AbilitySystemComponent->GetSet<UAttributeSetBase>();
	return IsValid(AttributeSet) ? AttributeSet->GetStrength() : -10.0f;
}



float ACharacterBase::GetGuard() const
{
	//return AttributeSetBase->GetGuard();

	const UAttributeSetBase* AttributeSet = AbilitySystemComponent->GetSet<UAttributeSetBase>();
	return AttributeSet ? AttributeSet->GetGuard() : -10.0f;
}



float ACharacterBase::GetMoveSpeed() const
{
	//return AttributeSetBase->GetMoveSpeed();

	const UAttributeSetBase* AttributeSet = AbilitySystemComponent->GetSet<UAttributeSetBase>();
	return AttributeSet ? AttributeSet->GetMoveSpeed() : -10.0f;
}



float ACharacterBase::GetAttackSpeed() const
{
	//return AttributeSetBase->GetAttackSpeed();

	const UAttributeSetBase* AttributeSet = AbilitySystemComponent->GetSet<UAttributeSetBase>();
	return AttributeSet ? AttributeSet->GetAttackSpeed() : -10.0f;
}



float ACharacterBase::GetDefense() const
{
	//return AttributeSetBase->GetDefense();

	const UAttributeSetBase* AttributeSet = AbilitySystemComponent->GetSet<UAttributeSetBase>();
	return AttributeSet ? AttributeSet->GetDefense() : -10.0f;
}



void ACharacterBase::HandleDamage(float DamageAmount, const FHitResult& HitResult, const FGameplayTagContainer& DamageTags, ACharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitResult, DamageTags, InstigatorCharacter, DamageCauser);
	OnHealthChangedDelegate.Broadcast(GetHealth());
}



void ACharacterBase::HandleHealthChanged(float DeltaHealth, const FGameplayTagContainer& SourceTags)
{
	if (!bDefaultAttributeInitialized) return;
	
	OnHealthChanged(DeltaHealth, SourceTags);
	OnHealthChangedDelegate.Broadcast(GetHealth());
}



void ACharacterBase::SetHealth(float NewHealth)
{
	const UAttributeSetBase* AttributeSet = AbilitySystemComponent->GetSet<UAttributeSetBase>();
	
	float DeltaHealth = AttributeSet->GetHealth();

	AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetHealthAttribute(),
		EGameplayModOp::Override, NewHealth);

	DeltaHealth = NewHealth - DeltaHealth;

	FGameplayTagContainer Container;
	OnHealthChanged(DeltaHealth, Container);
}



void ACharacterBase::UnEquipWeapon()
{
	if (IsValid(EquippedWeapon))
	{
		// Effect.WeaponStat 태그를 부여하는 이펙트를 제거
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(TEXT("Effect.WeaponStat")) });

		

		if (IsValid(EquippedWeapon->AdditionalEffectOnAttackClass))
		{
			SetAdditionalAttackEffectOnTarget(nullptr);
		}

		EquippedWeapon = nullptr;
	}
}



void ACharacterBase::UnEquipArmor()
{
	if (IsValid(EquippedArmor))
	{
		// Effect.ArmorStat 태그를 부여하는 이펙트를 제거
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(TEXT("Effect.ArmorStat")) });

		EquippedArmor = nullptr;
	}
}



void ACharacterBase::EquipWeapon(UItem_WeaponBase* WeaponToEquip)
{
	if (!IsValid(WeaponToEquip))
	{
		UE_LOG(LogTemp, Warning, TEXT("경고!!: %s 캐릭터가 장착할 무기 없음"), *GetName());
		return;
	}

	UnEquipWeapon();

	EquippedWeapon = WeaponToEquip;

	UGameInstanceBase* GIRef = GetGameInstance<UGameInstanceBase>();

	FGameplayEffectSpecHandle SpecHandle = GIRef->MakeWeaponStatEffect(EquippedWeapon->InternalName, AbilitySystemComponent);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	

	if (IsValid(EquippedWeapon->AdditionalEffectOnAttackClass))
	{
		SetAdditionalAttackEffectOnTarget(EquippedWeapon->AdditionalEffectOnAttackClass);
	}
}



void ACharacterBase::EquipArmor(UItem_ArmorBase* ArmorToEquip)
{
	if (!IsValid(ArmorToEquip))
	{
		UE_LOG(LogTemp, Warning, TEXT("경고!!: %s 캐릭터가 장착할 무기 없음"), *GetName());
		return;
	}

	UnEquipArmor();

	EquippedArmor = ArmorToEquip;

	UGameInstanceBase* GIRef = GetGameInstance<UGameInstanceBase>();

	FGameplayEffectSpecHandle SpecHandle = GIRef->MakeArmorStatEffect(EquippedArmor->InternalName, AbilitySystemComponent);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}



UItem_WeaponBase* ACharacterBase::GetEquippedWeapon() const
{
	return EquippedWeapon;
}



UItem_ArmorBase* ACharacterBase::GetEquippedArmor() const
{
	return EquippedArmor;
}



void ACharacterBase::OnStunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, nullptr);
	}
}



void ACharacterBase::HandleDie()
{
	if (bExplicitlyDead || AbilitySystemComponent->GetTagCount(DeadTag) > 0) return;

	bExplicitlyDead = true;

	AbilitySystemComponent->AddLooseGameplayTag(DeadTag);

	RemoveAbilities();

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->CancelAbilities();

		FGameplayTagContainer EffectsTagsToRemove;
		EffectsTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectsTagsToRemove);

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	DieAction();
}



void ACharacterBase::DieAction()
{
	StopAnimMontage();

	PlayAnimMontage(DeathMontage, 1.0f, DeathMontageStartSectionName);
}



void ACharacterBase::InitializeDefaultAttribute()
{
	if (bDefaultAttributeInitialized)
	{
		GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
		return;
	}

	// 상세 구현은 자식 클래스에서 수행함

	bDefaultAttributeInitialized = true;
}



void ACharacterBase::AddAbilities()
{
	if (!IsValid(AbilitySystemComponent) && AbilitySystemComponent->bAbilitiesWereGiven) return;

	for (auto& StartupAbility : Abilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility.Get(),
			GetCharacterLevel(), static_cast<int32>(-1), this));
	}

	AbilitySystemComponent->bAbilitiesWereGiven = true;
}



void ACharacterBase::RemoveAbilities()
{
	if (!IsValid(AbilitySystemComponent) && !AbilitySystemComponent->bAbilitiesWereGiven) return;

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;

	for (const auto& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && Abilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	int32 Length = AbilitiesToRemove.Num();

	for (int32 i = 0; i < Length; i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->bAbilitiesWereGiven = false;
}



void ACharacterBase::AddStartupEffects()
{
	if (!IsValid(AbilitySystemComponent) && AbilitySystemComponent->bIsStartupEffectsApplied) return;

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (const auto& GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle EffectHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect,
			GetCharacterLevel(), EffectContext);

		if (EffectHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectHandle.Data.Get(),
				AbilitySystemComponent);
		}
	}

	AbilitySystemComponent->bIsStartupEffectsApplied = true;
}



void ACharacterBase::SetGuardGuage(float NewGuard)
{
	if (!IsValid(AttributeSetBase)) return;

	//AttributeSetBase->SetGuard(NewGuard);

	float OldGuard = GetGuard();

	const UAttributeSetBase* AttributeSet = AbilitySystemComponent->GetSet<UAttributeSetBase>();

	AbilitySystemComponent->ApplyModToAttribute(AttributeSet->GetGuardAttribute(),
		EGameplayModOp::Override, NewGuard);

	OnGuardChanged(GetGuard() - OldGuard, FGameplayTagContainer());
}



void ACharacterBase::OnMoveSpeedAttributeChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
}



void ACharacterBase::ForceInitializeStats()
{
	if (IsValid(AbilitySystemComponent))
	{
		InitializeDefaultAttribute();
		AddStartupEffects();
		AddAbilities();
	}
}



void ACharacterBase::OnHealthChanged_Implementation(float DeltaHealth, const FGameplayTagContainer& SourceTags)
{
	if (!IsAlive()) HandleDie();
}



void ACharacterBase::OnGuardChanged_Implementation(float DeltaGuard, const FGameplayTagContainer& SourceTags)
{

}



void ACharacterBase::OnStrengthChanged_Implementation(float DeltaStrength, const FGameplayTagContainer& SourceTags)
{

}



void ACharacterBase::OnDefenseChanged_Implementation(float DeltaDefense, const FGameplayTagContainer& SourceTags)
{

}



void ACharacterBase::OnDamaged_Implementation(float DamageAmount, const FHitResult& HitResult, const FGameplayTagContainer& DamageTags, ACharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("%s: %s에 의해 피해 받음: %f"), *GetName(), DamageCauser != nullptr ? *DamageCauser->GetName() : *FString(TEXT("NONE")), DamageAmount));
}