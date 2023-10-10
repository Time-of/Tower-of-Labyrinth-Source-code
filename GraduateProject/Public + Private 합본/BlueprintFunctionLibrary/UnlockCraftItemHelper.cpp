
#include "BlueprintFunctionLibrary/UnlockCraftItemHelper.h"

#include "GameInstance/GISS_InteractableNpcData.h"



void UUnlockCraftItemHelper::UnlockCraftItem(AActor* WorldContext, const FPrimaryAssetId& Item)
{
	if (!IsValid(WorldContext)) return;

	if (UGameInstance* GI = WorldContext->GetGameInstance())
	{
		UGISS_InteractableNpcData* NpcDataSS = GI->GetSubsystem<UGISS_InteractableNpcData>();

		// 제작 아이템 활성화
		if (IsValid(NpcDataSS) && Item.IsValid())
		{
			NpcDataSS->ShopCraftItem_CanBeCraftedMap.Add(Item, true);
		}
	}
}
