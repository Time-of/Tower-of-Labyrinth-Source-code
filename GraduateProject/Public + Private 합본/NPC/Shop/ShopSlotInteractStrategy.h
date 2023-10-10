
#pragma once

#include "CoreMinimal.h"



/**
 * @author 이성수
 * @brief 상점 슬롯의 상호작용 스트래티지(전략) 기본 클래스입니다.
 * @since 2023-03-13
 */
class GRADUATEPROJECT_API ShopSlotInteractStrategy
{
public:
	ShopSlotInteractStrategy();
	virtual ~ShopSlotInteractStrategy();

	/**
	* @brief 상호작용을 시도합니다.
	* @return 상호작용 성공 여부입니다.
	* @param Item 상호작용 대상 아이템
	* @param[inout] CurrentItemQuantity 현재 아이템 수량을 레퍼런스로 전달합니다. 이 값은 수정됩니다.
	* @param InteractQuantity 상호작용할 수량
	*/
	virtual bool TryInteract(class UNPCShopItemSlot* Context, class UItemBase* Item, int& CurrentItemQuantity, int InteractQuantity);
};
