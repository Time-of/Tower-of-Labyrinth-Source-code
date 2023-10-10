﻿
#pragma once

#include "CoreMinimal.h"

#include "NPC/Shop/ShopSlotInteractStrategy.h"



/**
 * @author 이성수
 * @brief 상점 구매 스트래티지 클래스입니다.
 * @since 2023-03-13
 */
class GRADUATEPROJECT_API ShopSlotBuyStrategy : public ShopSlotInteractStrategy
{
public:
	ShopSlotBuyStrategy();

	bool TryInteract(class UNPCShopItemSlot* Context, class UItemBase* Item, int& CurrentItemQuantity, int InteractQuantity) final;
};
