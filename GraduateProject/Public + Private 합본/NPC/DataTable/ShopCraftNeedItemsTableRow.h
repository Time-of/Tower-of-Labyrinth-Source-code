
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ShopCraftNeedItemsTableRow.generated.h"



/**
 * @author 이성수
 * @brief 제작에 필요한 아이템과 그 수량을 묶는 구조체입니다.
 * @since 2023-03-06
 */
USTRUCT(BlueprintType)
struct FCraftNeedItem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPrimaryAssetId NeedItem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NeedQuantity = 1;
};



/**
 * @author 이성수
 * @brief 상점 NPC의 제작 시스템에 사용되는 데이터 테이블 행 구조체입니다.
 * @details 제작 아이템과, 필요 아이템/필요 수량의 배열로 이루어져 있습니다.
 * @since 2023-03-06
 */
USTRUCT(BlueprintType)
struct GRADUATEPROJECT_API FShopCraftNeedItemsDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** @brief 제작할 아이템 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPrimaryAssetId ItemToCraft;

	/** @brief 필요 아이템, 필요 수량 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FCraftNeedItem> NeedItems;
};