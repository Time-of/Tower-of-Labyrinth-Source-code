
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameInstance/GISS_TemporaryPlayerData.h"
#include "LevelEvents/InteractableLevelEventBase.h"
#include "LvEvent_RandomReinforceStats.generated.h"



UENUM(BlueprintType)
enum class ERandomReinforceStatType : uint8
{
	STRENGTH UMETA(DisplayName = "힘"),
	MAXHEALTH UMETA(DisplayName = "최대 체력"),
	MOVESPEED UMETA(DisplayName = "이동 속도"),
	REDUCESKILLCOST UMETA(DisplayName = "스킬 마나 소모량 감소")
};



USTRUCT(BlueprintType)
struct GRADUATEPROJECT_API FLvEventRandomReinforceStatTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** @brief 무작위 선택 가중치 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Weight = 0.0f;

	/** @brief 적용할 이펙트 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FReinforceEffectData ReinforceData;

	/**
	* @brief 적용할 이펙트 스탯 타입
	* @details 부여 완료 후 어떤 스탯이 부여되었는지 위젯 표시용으로 사용
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ERandomReinforceStatType ReinforceStatType = ERandomReinforceStatType::MAXHEALTH;
};



/**
 * @author 이성수 
 * @brief 레벨 이벤트 중, 무작위 능력치 강화를 부여하는 이벤트 액터입니다.
 * @since 2023-04-27
 */
UCLASS()
class GRADUATEPROJECT_API ALvEvent_RandomReinforceStats : public AInteractableLevelEventBase
{
	GENERATED_BODY()

public:
	ALvEvent_RandomReinforceStats();



protected:
	UFUNCTION()
	void OnConfirmButtonClicked();

	void AssignEffect(const TSubclassOf<class UGameplayEffect>& ReinforceEffect, float ReinforceAmount, ERandomReinforceStatType AssignedStatType);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAssignCompleted(ERandomReinforceStatType AssignedStatType);

	void OnInteractUIShown() final;

	void OnInteractUIBeforeHidden() final;

	bool CanInteract_Implementation() override;



	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UDataTable* ReinforceTable;

	bool bWasAssigned;
};
