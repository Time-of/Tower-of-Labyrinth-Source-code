
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemyDropTableRow.generated.h"



USTRUCT(BlueprintType)
struct FDropItemInfo
{
	GENERATED_BODY()

public:
	/** @brief 드랍 확률 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DropChance = 0.0f;

	/** @brief 드랍할 아이템 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPrimaryAssetId DropItem;

	/** @brief 드랍하는 개수 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 DropAmount = 1;
};



/**
 * @author 이성수
 * @brief 적 캐릭터 드랍 테이블의 행 구조체입니다.
 * @since 2023-02-24
 */
USTRUCT(BlueprintType)
struct GRADUATEPROJECT_API FEnemyDropTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** @brief 돈 드랍 확률 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MoneyDropChance = 1.0f;

	/** @brief 돈 드랍 수량 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MoneyDropAmount = 1.0f;

	/** @brief 드랍할 아이템 정보들 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDropItemInfo> DropItems;
};
