
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GISS_PlayerMoney.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoneyChangedDelegate, float, NewMoneyValue);



/**
 * @author 이성수
 * @brief 플레이어의 돈을 관리하는 게임인스턴스 서브시스템 클래스입니다.
 * @since 2023-02-17
 */
UCLASS()
class GRADUATEPROJECT_API UGISS_PlayerMoney : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGISS_PlayerMoney();

	/**
	* @brief 돈을 추가합니다.
	* 
	* @param Amount 추가할 양
	*/
	UFUNCTION(BlueprintCallable, Category = "Player|Money")
	void AddMoney(float Amount);

	/**
	* @brief 돈을 가져옵니다.
	* @return 현재 돈(Money)
	*/
	UFUNCTION(BlueprintCallable, Category = "Player|Money")
	float GetMoney() const;

	/**
	* @brief 돈을 설정합니다.
	* 
	* @param NewAmount 새로 설정할 값
	*/
	UFUNCTION(BlueprintCallable, Category = "Player|Money")
	void SetMoney(float NewAmount);

	/**
	* @brief 돈 사용을 **시도**합니다.
	* @details true를 반환했다면, 이미 돈을 사용한 것입니다. false를 반환했다면, 현재 돈은 그대로 있는 상태입니다.
	* @details 예를 들어 TryUseMoney(200.0f) 형태로 사용한다면, Money가 200.0f 이상인 경우 Money에서 200.0f를 삭감하고 true를 반환합니다.
	* @return 사용에 성공한 경우 true, 실패한 경우 false
	* 
	* @param Amount 사용할 양
	*/
	UFUNCTION(BlueprintCallable, Category = "Player|Money")
	bool TryUseMoney(float Amount);



	UPROPERTY(BlueprintAssignable, Category = "Player|Money")
	FMoneyChangedDelegate OnMoneyChanged;

	/** @brief 적이 드랍할 돈 클래스입니다. */
	UPROPERTY(BlueprintReadOnly, Category = "Player|Money|Pickup")
	TSubclassOf<class AMoneyPickupBase> MoneyPickupClass;
	
	UPROPERTY()
	USoundBase* UseMoneySound;



private:
	UPROPERTY()
	float Money;
};
