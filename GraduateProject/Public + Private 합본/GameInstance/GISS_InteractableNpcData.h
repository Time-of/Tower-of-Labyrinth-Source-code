
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/PrimaryAssetId.h"
#include "GISS_InteractableNpcData.generated.h"

class UItemBase;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShopBuyItemQuantityMapChanged, FPrimaryAssetId, Item, int32, NewQuantity);



/**
 * @author 이성수
 * @brief 상호작용 NPC들이 사용할 데이터를 저장할 게임인스턴스 서브시스템 클래스입니다.
 * @since 2023-02-27
 * @see USaveGame_Shop 참고.
 */
UCLASS()
class GRADUATEPROJECT_API UGISS_InteractableNpcData : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGISS_InteractableNpcData();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	* @brief 아이템 제작에 필요한 아이템 배열을 가져옵니다.
	* @details #include "ShopCraftNeedItemsDataTableRow.h" 필요.
	* @see FShopCraftNeedItemsDataTableRow 참고.
	* @return 찾은 필요 아이템 배열
	* @param InternalItemName 행 이름: 아이템의 내부 이름 (ItemBase의 InternalName)
	*/
	TArray<struct FCraftNeedItem> GetCraftNeedItems(const FName& InternalItemName) const;
	
	UFUNCTION(BlueprintCallable)
	void InitializeShopData();

	UFUNCTION(BlueprintCallable)
	void SaveNpcData(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void LoadNpcData(int32 SlotIndex);

	void SetShopBuyItemMap(const FPrimaryAssetId& Item, int32 NewQuantity);



	FShopBuyItemQuantityMapChanged OnShopBuyItemQuantityChanged;

	UPROPERTY()
	TMap<FPrimaryAssetId, int32> ShopBuyItemQuantityMap;

	UPROPERTY()
	TMap<FPrimaryAssetId, int32> ShopCraftItemQuantityMap;

	/** 
	* @brief 상점 아이템의 제작 가능 여부 맵
	* @details 만약 설정되어 있지 않다면 false(제작 불가)인 것으로 간주
	*/
	UPROPERTY()
	TMap<FPrimaryAssetId, bool> ShopCraftItem_CanBeCraftedMap;


private:
	UPROPERTY()
	class UDataTable* ShopDataTable;

	UPROPERTY()
	class UDataTable* ShopCraftNeedItemsDataTable;
};
