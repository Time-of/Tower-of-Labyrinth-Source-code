
#include "Projectile/ProjectileBase.h"

#include "CharacterBase.h"
#include "Enemy/EnemyCharacterBase.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"



AProjectileBase::AProjectileBase() : Super(),
	bUseOverlapMethod(true),
	bIgnoreInstigator(true), bAffectsPlayer(true), bAffectsMonster(false),
	SpawnOnDestroyedParticle(nullptr),
	SpawnOnDestroyedNiagara(nullptr),
	CamShakeOnDestroyedClass(nullptr),
	CamShakeRadius(1000.0f),
	CollisionSphereComp(nullptr),
	ProjectileMovementComp(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 5.0f;

	CollisionSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComp->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	SetRootComponent(CollisionSphereComp);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComp->InitialSpeed = 1000.0f;
	ProjectileMovementComp->MaxSpeed = 1000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	ProjectileMovementComp->bSweepCollision = false;
	AddOwnedComponent(ProjectileMovementComp);
}



void AProjectileBase::SetEffectSpecHandlesToApply(const TArray<struct FGameplayEffectSpecHandle>& NewEffectSpecHandlesToApply)
{
	EffectSpecHandlesToApply = NewEffectSpecHandlesToApply;
}



UProjectileMovementComponent* AProjectileBase::GetProjectileMovement() const
{
	return ProjectileMovementComp;
}



void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	//SetIgnoreInstigator(bIgnoreInstigator);

	if (bUseOverlapMethod)
	{
		CollisionSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnCollsionSphereBeginOverlap);
	}
	else
	{
		CollisionSphereComp->SetCollisionProfileName(TEXT("Projectile"));
		CollisionSphereComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnCollisionSphereHit);
		ProjectileMovementComp->bSweepCollision = true;
	}
}



void AProjectileBase::Destroyed()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnOnDestroyedParticle, GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnOnDestroyedNiagara, GetActorLocation());
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), CamShakeOnDestroyedClass, GetActorLocation(), 0.0f, CamShakeRadius);

	Super::Destroyed();
}



void AProjectileBase::OnConstruction(const FTransform& Transform)
{
	if (bUseOverlapMethod)
	{
		CollisionSphereComp->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	}
	else
	{
		CollisionSphereComp->SetCollisionProfileName(TEXT("Projectile"));
	}
}



void AProjectileBase::SetIgnoreInstigator(bool NewActive)
{
	bIgnoreInstigator = NewActive;
	
	CollisionSphereComp->IgnoreActorWhenMoving(GetInstigator(), bIgnoreInstigator);
	bIgnoreInstigator ? GetInstigator()->MoveIgnoreActorAdd(this) : GetInstigator()->MoveIgnoreActorRemove(this);
}



void AProjectileBase::SetIgnoreInstigator()
{
	CollisionSphereComp->IgnoreActorWhenMoving(GetInstigator(), bIgnoreInstigator);
	bIgnoreInstigator ? GetInstigator()->MoveIgnoreActorAdd(this) : GetInstigator()->MoveIgnoreActorRemove(this);
}



void AProjectileBase::OnCollisionSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!IsValid(OtherActor))
	{
		Destroy();
		return;
	}

	// (bAffectsPlayer일 때) 맞은 대상이 플레이어거나 (bAffectsMonster일 때) 몬스터라면
	if ((bAffectsPlayer && OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()) || (bAffectsMonster && OtherActor->IsA<AEnemyCharacterBase>()))
	{
		if (ACharacterBase* OtherCharacter = Cast<ACharacterBase>(OtherActor))
		{
			for (const FGameplayEffectSpecHandle& Handle : EffectSpecHandlesToApply)
			{
				OtherCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());
			}

			Destroy();
		}
	}
	else
	{
		Destroy();
	}
}



void AProjectileBase::OnCollsionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// (bAffectsPlayer일 때) 맞은 대상이 플레이어거나 (bAffectsMonster일 때) 몬스터라면
	if ((bAffectsPlayer && OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()) || (bAffectsMonster && OtherActor->IsA<AEnemyCharacterBase>()))
	{
		if (ACharacterBase* OtherCharacter = Cast<ACharacterBase>(OtherActor))
		{
			for (const FGameplayEffectSpecHandle& Handle : EffectSpecHandlesToApply)
			{
				OtherCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());
			}

			Destroy();
		}
	}
	else if (!bAffectsPlayer || !OtherActor->ActorHasTag(TEXT("Hostile")))
	{
		Destroy();
	}
}
