
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TryActivateRandomAbility.generated.h"



/**
 * @author 이성수
 * @brief 가중치 변수와 어빌리티 클래스 배열을 가지는 구조체입니다.
 * @see UBTTask_TryActivateRandomAbility 참고.
 * @since 2023-02-08
 */
USTRUCT(BlueprintType)
struct FWeightedAbilityContainer
{
	GENERATED_BODY()

	/**
	* @brief 가중치
	*/
	UPROPERTY(EditAnywhere, Category = "Ability")
	float Weight = 1.0f;

	/**
	* @brief 어빌리티 클래스 배열
	*/
	UPROPERTY(EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<class UGameplayAbility>> AbilityClassArray;
};



/**
 * @author 이성수
 * @brief 무작위로 가중치에 따라 어빌리티 사용을 시도하는 BTTask 입니다.
 * @details 어빌리티 사용에 성공한 경우에만 Successed를 반환하고, 실패한 경우에는 Failed를 반환합니다.
 * @see ExecuteTask() 참고.
 * @date 2023-02-01
 * @version 0.0.2
 */
UCLASS()
class GRADUATEPROJECT_API UBTTask_TryActivateRandomAbility : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_TryActivateRandomAbility();

	/**
	* @return 어빌리티 사용에 성공한 경우에만 Successed를 반환하고, 실패한 경우에는 Failed를 반환합니다.
	* @return 또는 가중치 컨테이너가 유효하지 않다고 판단한 경우, Failed를 반환합니다.
	*/
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;



protected:
	/**
	* @brief 배열에서 무작위 어빌리티 클래스를 반환합니다.
	* @return 배열 내의 무작위 TSubclassOf<UGameplayAbility>
	*/
	TSubclassOf<class UGameplayAbility> GetRandomAbilityInArray(TArray<TSubclassOf<class UGameplayAbility>> AbilityClassArray);

	/**
	* @brief 가중치 컨테이너 안에서 가중치에 따라 무작위로 어빌리티 클래스의 배열을 가져옵니다.
	* @return 성공적으로 가져온 경우 true, 유효하지 않다고 판단했다면(가중치 총합이 0.0f 이하라면) false
	*/
	bool GetRandomAbilityClassArrayInWeightedContainer(FWeightedAbilityContainer& OutContainer);

	/**
	* @brief 폐기된 기능입니다.
	*/
	void SwapContainer(FWeightedAbilityContainer& A, FWeightedAbilityContainer& B);

	/**
	* @brief 폐기된 기능입니다.
	*/
	void MakeHeapTree(int Parent, int32 Size);

	UPROPERTY(EditAnywhere, Category = "Ability")
	TArray<FWeightedAbilityContainer> AbilityContainerArray;
};
