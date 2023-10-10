
#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "Item_MaterialBase.generated.h"



/**
 * @author 이성수
 * @brief 재료 아이템 베이스 클래스
 * @since 2023-02-23
 */
UCLASS()
class GRADUATEPROJECT_API UItem_MaterialBase : public UItemBase
{
	GENERATED_BODY()
	
public:
	UItem_MaterialBase();
};
