
#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include <GraduateProject/GraduateProject.h>

#include "ShopDataTableRow.generated.h"



/**
 * @author 이성수
 * @brief 상점 NPC에 사용될 데이터 테이블 행 구조체입니다.
 * @since 2023-02-27
 */
USTRUCT(BlueprintType)
struct GRADUATEPROJECT_API FShopDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** @brief 상호작용(구매/제작) 타입, **판매 선택하지 말 것! 반영 안 됨** */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EShopInteractType InteractType = EShopInteractType::NONE;

	/** @brief 아이템 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPrimaryAssetId Item;

	/** @brief 아이템 수량 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ItemQuantity = 0;
};