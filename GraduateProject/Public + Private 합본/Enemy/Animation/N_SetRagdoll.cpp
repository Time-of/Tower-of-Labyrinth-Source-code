
#include "Enemy/Animation/N_SetRagdoll.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"



UN_SetRagdoll::UN_SetRagdoll() : Super(), bEnableRagdoll(true)
{
	
}



void UN_SetRagdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (bEnableRagdoll)
		{
			Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			Character->GetMesh()->SetCollisionProfileName(FName(TEXT("Ragdoll")));
			Character->GetMesh()->SetSimulatePhysics(true);
			Character->GetCharacterMovement()->DisableMovement();
		}
		else
		{
			Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			Character->GetMesh()->SetCollisionProfileName(FName(TEXT("NoCollision")));
			Character->GetMesh()->SetSimulatePhysics(false);
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}

	Super::Notify(MeshComp, Animation, EventReference);
}



FString UN_SetRagdoll::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("래그돌 활성화: %s"), bEnableRagdoll ? *FString(TEXT("true")) : *FString(TEXT("false")));
}
