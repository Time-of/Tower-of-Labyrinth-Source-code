
#include "Item/ItemPickupBase.h"

#include "CharacterBase.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Item/ItemBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"



AItemPickupBase::AItemPickupBase() : Super(),
	ItemToPickup(nullptr),
	ItemQuantity(0),
	EffectClassToApplyToTarget(nullptr), EffectLevel(1)
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetCapsuleHalfHeight(90.0f);
	CapsuleComp->SetCapsuleRadius(15.0f);
	CapsuleComp->SetCollisionProfileName(TEXT("NoCollision"));
	//CapsuleComp->SetupAttachment(GetRootComponent());
	SetRootComponent(CapsuleComp);

	ItemStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	ItemStaticMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	ItemStaticMeshComp->SetupAttachment(CapsuleComp);

	PickupRadiusSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphereComp"));
	PickupRadiusSphereComp->SetSphereRadius(128.0f);
	PickupRadiusSphereComp->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	PickupRadiusSphereComp->SetupAttachment(CapsuleComp);
}



void AItemPickupBase::SetPickupItemInfo(UItemBase* Item, int32 NewQuantity)
{
	ItemToPickup = Item;
	ItemQuantity = NewQuantity;
}



void AItemPickupBase::BeginPlay()
{
	Super::BeginPlay();

	PickupRadiusSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItemPickupBase::OnSphereBeginOverlap);
}



void AItemPickupBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		GiveItem(OtherActor);
	}
}



void AItemPickupBase::ApplyEffectToTarget_Implementation(AActor* TargetActor)
{
	if (ACharacterBase* TargetCharacter = Cast<ACharacterBase>(TargetActor))
	{
		UAbilitySystemComponent* ASC = TargetCharacter->GetAbilitySystemComponent();

		if (ASC != nullptr && IsValid(EffectClassToApplyToTarget))
		{
			FGameplayEffectSpecHandle Handle = ASC->MakeOutgoingSpec(EffectClassToApplyToTarget, EffectLevel, ASC->MakeEffectContext());

			ASC->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());
		}
	}
}
