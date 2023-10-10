
#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "ItemSlot.generated.h"



/**
 * @author 이성수
 * @brief 인벤토리에 사용되는 아이템 슬롯 클래스입니다.
 * @since 2023-03-03
 * @see UGISS_PlayerInventory 참고.
 */
USTRUCT(BlueprintType)
struct GRADUATEPROJECT_API FItemSlot
{
	GENERATED_BODY()

public:
	FItemSlot() : Item(nullptr), Quantity(0), MaxQuantity(0)
	{

	}

	FItemSlot(UItemBase* NewItem, int32 NewQuantity) : Item(NewItem), Quantity(NewQuantity)
	{
		// 아이템의 최대 수량이 0 미만이라면, 개수 제한 없음
		MaxQuantity = NewItem != nullptr ?
			(NewItem->MaxCount < 0 ? INT32_MAX : NewItem->MaxCount) : 0;
	}

	void UpdateItemSlot(UItemBase* NewItem, int32 NewQuantity)
	{
		Item = NewItem;

		// 아이템의 최대 수량이 0 미만이라면, 개수 제한 없음
		MaxQuantity = NewItem != nullptr ?
			(NewItem->MaxCount < 0 ? INT32_MAX : NewItem->MaxCount) : 0;

		Quantity = NewQuantity;
	}

	/**
	* @brief 수량을 추가합니다. 가급적이면 UGISS_PlayerInventory::AddItem()을 사용하세요. (이 함수는 관련 콜백을 발생시키지 않기 때문)
	*/
	void AddQuantity(int32 QuantityToAdd)
	{
		Quantity = FMath::Max(Quantity + QuantityToAdd, 0);
	}

	UPROPERTY()
	UItemBase* Item;

	UPROPERTY()
	int32 Quantity;

	UPROPERTY()
	int32 MaxQuantity;
};
