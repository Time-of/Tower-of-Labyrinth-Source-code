
#include "Enemy/AI/BTTask_TryActivateRandomAbility.h"

#include "Enemy/EnemyAIControllerBase.h"
#include "Enemy/EnemyCharacterBase.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "Enemy/Abilities/GameplayAbilityBase.h"

#include "Kismet/KismetMathLibrary.h"



UBTTask_TryActivateRandomAbility::UBTTask_TryActivateRandomAbility() : Super()
{
	NodeName = TEXT("무작위 어빌리티 활성 시도");
}



EBTNodeResult::Type UBTTask_TryActivateRandomAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyCharacterBase* EnemyCharacter = OwnerComp.GetAIOwner()->GetPawn<AEnemyCharacterBase>())
	{
		FWeightedAbilityContainer SelectedContainer;

		if (!GetRandomAbilityClassArrayInWeightedContainer(SelectedContainer)) return EBTNodeResult::Failed;



		bool bSuccessed = EnemyCharacter->GetAbilitySystemComponent()->TryActivateAbilityByClass(GetRandomAbilityInArray(SelectedContainer.AbilityClassArray));

		return bSuccessed ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}



FString UBTTask_TryActivateRandomAbility::GetStaticDescription() const
{
	return FString::Printf(TEXT("무작위 어빌리티 사용 시도"));
}



TSubclassOf<class UGameplayAbility> UBTTask_TryActivateRandomAbility::GetRandomAbilityInArray(TArray<TSubclassOf<UGameplayAbility>> AbilityClassArray)
{
	int32 Length = AbilityClassArray.Num();

	return AbilityClassArray[UKismetMathLibrary::RandomIntegerInRange(0, Length - 1)];
}



bool UBTTask_TryActivateRandomAbility::GetRandomAbilityClassArrayInWeightedContainer(FWeightedAbilityContainer& OutContainer)
{
	int32 Size = AbilityContainerArray.Num();

	/*
	for (int i = Size / 2 - 1; i >= 0; i--)
	{
		MakeHeapTree(i, Size);
	}

	for (int i = Size - 1; i >= 0; i--)
	{
		SwapContainer(AbilityContainerArray[0], AbilityContainerArray[i]);
		MakeHeapTree(0, i);
	}
	*/



	float TotalWeight = 0.0f;

	for (const FWeightedAbilityContainer& WeightedContainer : AbilityContainerArray)
	{
		TotalWeight += WeightedContainer.Weight;
	}

	if (TotalWeight <= 0.0f) return false;



	float Pivot = UKismetMathLibrary::RandomFloatInRange(0.0f, TotalWeight);

	for (const FWeightedAbilityContainer& WeightedContainer : AbilityContainerArray)
	{
		if (Pivot < WeightedContainer.Weight)
		{
			OutContainer = WeightedContainer;
			return true;
		}
		else Pivot -= WeightedContainer.Weight;
	}

	OutContainer = AbilityContainerArray[Size - 1];
	return true;
}



void UBTTask_TryActivateRandomAbility::SwapContainer(FWeightedAbilityContainer& A, FWeightedAbilityContainer& B)
{
	FWeightedAbilityContainer TempContainer = A;
	A = B;
	B = TempContainer;
}



void UBTTask_TryActivateRandomAbility::MakeHeapTree(int Parent, int32 Size)
{
	int Largest = Parent;

	// 배열 인덱스는 0부터 시작
	int LeftChild = Parent * 2 + 1;
	int RightChild = Parent * 2 + 2;

	if (LeftChild < Size && AbilityContainerArray[LeftChild].Weight > AbilityContainerArray[Parent].Weight)
	{
		Largest = LeftChild;
	}

	if (RightChild < Size && AbilityContainerArray[RightChild].Weight > AbilityContainerArray[Parent].Weight)
	{
		Largest = RightChild;
	}

	if (Largest != Parent)
	{
		SwapContainer(AbilityContainerArray[Parent], AbilityContainerArray[Largest]);

		MakeHeapTree(Largest, Size);
	}
}
