
#pragma once

#include "CoreMinimal.h"

#include "Widget/NPCShopItemSlot.h"
#include "NPC/DataTable/ShopCraftNeedItemsTableRow.h"

#include "NPCShopCraftSlot.generated.h"



/**
 * @author 이성수
 * @brief **(2023-03-13 폐기됨) 스트래티지 패턴을 사용하도록 리팩토링했기 때문에, 폐기됨.**
 * @details 상점 제작 슬롯 클래스입니다. 기본적으로는 상점 아이템 슬롯 클래스를 상속받기 때문에, 슬롯 클래스의 기능을 모두 가지고 있습니다.
 * @date 2023-03-06
 * @version 0.0.2
 */
UCLASS()
class GRADUATEPROJECT_API UNPCShopCraftSlot : public UNPCShopItemSlot
{
	GENERATED_BODY()

public:
	UNPCShopCraftSlot(const FObjectInitializer& ObjectInitializer);

	[[deprecated("스트래티지 패턴을 사용하도록 리팩토링했기에, 사용하지 않음")]]
	void SetNeedItems(const TArray<FCraftNeedItem>& NewNeedItems);

	[[deprecated("스트래티지 패턴을 사용하도록 리팩토링했기에, 사용하지 않음")]]
	inline TArray<FCraftNeedItem> GetNeedItems() const;

	//virtual bool CraftItem(int32 CraftCount = 1) override;
	


protected:
	TArray<FCraftNeedItem> NeedItems;
};
