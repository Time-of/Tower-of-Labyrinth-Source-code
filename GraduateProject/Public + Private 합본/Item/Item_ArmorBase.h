
#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "Item_ArmorBase.generated.h"



/**
 * @author 이성수
 * @brief 방어구 아이템 베이스 클래스
 * @since 2023-02-23
 */
UCLASS()
class GRADUATEPROJECT_API UItem_ArmorBase : public UItemBase
{
	GENERATED_BODY()

public:
	UItem_ArmorBase();

	bool TryEquipItem() override;
};
