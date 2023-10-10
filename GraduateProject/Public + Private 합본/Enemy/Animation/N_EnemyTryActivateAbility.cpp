
#include "Enemy/Animation/N_EnemyTryActivateAbility.h"

#include "Enemy/EnemyCharacterBase.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Abilities/GameplayAbilityBase.h"




UN_EnemyTryActivateAbility::UN_EnemyTryActivateAbility() : Super(), bPrintDebugMessage(false)
{
	
}



void UN_EnemyTryActivateAbility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AEnemyCharacterBase* EnemyCharacter = MeshComp->GetOwner<AEnemyCharacterBase>())
	{
		if (UAbilitySystemComponent* ASC = EnemyCharacter->GetAbilitySystemComponent())
		{
			bool bSuccessed = ASC->TryActivateAbilityByClass(AbilityClassToActivate);

			if (bPrintDebugMessage)
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("%s: 어빌리티 %s 발동 시도: %s"),
					*GetName(),
					IsValid(AbilityClassToActivate) ? *AbilityClassToActivate->GetName() : *FString(TEXT("없음")),
					bSuccessed ? *FString(TEXT("성공")) : *FString(TEXT("실패"))));
		}
	}

	Super::Notify(MeshComp, Animation, EventReference);
}



FString UN_EnemyTryActivateAbility::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("%s 어빌리티 사용"), IsValid(AbilityClassToActivate) ? *AbilityClassToActivate->GetName() : *FString(TEXT("없음")));
}
