
#pragma once

#include "CoreMinimal.h"

#include "Widget/NPCShopItemDetails_Simplified.h"

#include <GraduateProject/GraduateProject.h>

#include "NPCShopItemDetails.generated.h"



/**
 * @author 이성수
 * @brief 상점 NPC의 UI에서 보여줄 아이템 디테일 화면 위젯 클래스입니다.
 * @since 2023-02-26
 * @todo [완료] 재료 아이템 읽어서 버티컬 박스에 동적으로 추가/제거하는 작업 필요!!
 */
UCLASS()
class GRADUATEPROJECT_API UNPCShopItemDetails : public UNPCShopItemDetails_Simplified
{
	GENERATED_BODY()

public:
	UNPCShopItemDetails(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	/**
	* @brief 아이템 정보를 업데이트합니다.
	* @param Item 아이템 정보
	*/
	virtual void UpdateItem(const class UItemBase* Item) override;

	void UpdateItem(const class UItemBase* Item, const TArray<struct FCraftNeedItem>& CraftNeedItemArray);

	void UpdateTextUnknownItem();

	void UpdateMoneyText(float NewMoney);

	void SetCraftNeedItems(const TArray<struct FCraftNeedItem>& NeedItems);

	void RemoveCraftNeedItems();

	

	/**
	* @brief 상호작용 타입에 따라 상호작용 텍스트를 설정합니다. (SetText)
	* @param Type 상호작용 타입
	*/
	UFUNCTION(BlueprintCallable, Category = "ItemDetails")
	void SetInteractTextByType(EShopInteractType Type);

	void SetInteractTextVisibility(ESlateVisibility NewVisibility);



protected:
	UFUNCTION()
	void AfterVisibilityChanged(ESlateVisibility NewVisibility);

	void StartAutoScroll();



	FTimerHandle AutoScrollTimerHandle;



	UPROPERTY(BlueprintReadWrite, Category = "ItemDetails", meta = (BindWidget))
	class UScrollBox* NeedItemsScrollBox;

	/** @brief 필요한 돈(골드) 위젯 */
	UPROPERTY(BlueprintReadWrite, Category = "ItemDetails", meta = (BindWidget))
	class UNPCShopNeedItemInfo* NeedInfo_Money;

	/** @brief 필요한 아이템 목록 버티컬 박스 */
	UPROPERTY(BlueprintReadWrite, Category = "ItemDetails", meta = (BindWidget))
	class UVerticalBox* VerticalBox_NeedItems;

	UPROPERTY(BlueprintReadWrite, Category = "ItemDetails", meta = (BindWidget))
	class UTextBlock* InteractTextBlock;

	UPROPERTY(BlueprintReadWrite, Category = "ItemDetails", meta = (BindWidget))
	class UBorder* InteractBorder;

	UPROPERTY()
	TSubclassOf<class UNPCShopNeedItemInfo> NeedInfoClass;

	float AutoScrollElapsedTime;
};
