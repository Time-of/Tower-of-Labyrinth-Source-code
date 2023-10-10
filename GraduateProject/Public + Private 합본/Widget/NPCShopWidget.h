
#pragma once

#include "CoreMinimal.h"

#include "Widget/InteractableNPCWidgetBase.h"

#include <GraduateProject/GraduateProject.h>

#include "NPCShopWidget.generated.h"

class UUniformGridPanel;



UENUM(BlueprintType)
enum class EShopItemDisplayedType : uint8
{
	WEAPONS_ONLY, ARMORS_ONLY
};



/**
 * @author 이성수
 * @brief 상점 NPC가 사용하는 위젯 클래스입니다.
 * @since 2023-02-18
 */
UCLASS()
class GRADUATEPROJECT_API UNPCShopWidget : public UInteractableNPCWidgetBase
{
	GENERATED_BODY()

public:
	UNPCShopWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;



protected:
	/**
	* @brief 아이템 슬롯을 생성합니다.
	* @details ItemSlotRow만큼의 행, ItemSlotColumn만큼의 열을 가진 슬롯을 UniformGrid에 자식으로써 생성합니다.
	* @param UniformGrid 이 유니폼 그리드의 자식 위젯으로써 생성
	* @param ItemSlotClass 생성할 아이템 슬롯 위젯 클래스
	* @param InteractType 상호작용 타입
	*/
	UFUNCTION(BlueprintCallable, Category = "Shop|Setup")
	void SetupItemSlots(UUniformGridPanel* UniformGrid, TSubclassOf<class UNPCShopItemSlot> ItemSlotClass, EShopInteractType InteractType);

	/** @brief 유니폼 그리드에 있는 아이템 슬롯들을 제거합니다. NativeDestruct에서 호출합니다. (블루프린트 X) */
	void RemoveItemSlots(UUniformGridPanel* UniformGrid);

	/** @brief 아이템 데이터를 UGISS_InteractableNPCData로부터 가져와 해당 슬롯의 아이템을 설정합니다. NativeConstruct에서 호출합니다. (블루프린트 X) */
	void SetupShopItemsData();
	
	/** @brief 인벤토리로부터 아이템을 로드해 판매 슬롯에 설정합니다. */
	void LoadSellItemFromInventory();

	void ForceLoadSellItem();



	/**
	* @brief 아이템 디테일 위젯을 업데이트하고, 보여줍니다. 콜백으로 작동합니다.
	* @details 아이템이 유효하지 않다면, 작동하지 않습니다.
	* @param Item 슬롯의 아이템
	* @param InteractType 이 슬롯이 구매/판매/제작 중 어떤 타입인가
	*/
	UFUNCTION()
	void ShowItemDetailsWidget(class UItemBase* Item, EShopInteractType InteractType);

	/** @brief 아이템 디테일 위젯을 감춥니다. */
	UFUNCTION()
	void HideItemDetailsWidget(class UItemBase* Item = nullptr);

	/** @brief 돈 텍스트를 업데이트합니다. */
	UFUNCTION()
	void UpdateMoneyText(float NewAmount);

	UFUNCTION()
	void OnInventoryUpdated(class UItemBase* Item, int32 NewItemCount, int32 DeltaQuantity);



	/** @brief 인덱스로 슬롯에 아이템을 설정합니다. */
	UFUNCTION(BlueprintCallable, Category = "Shop")
	UNPCShopItemSlot* SetItemSlotByIndex(UUniformGridPanel* UniformGrid, class UItemBase* ItemToSet, int32 NewItemCount, int32 Index);



	// 행
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Shop|Setup")
	int32 ItemSlotRow;

	// 열
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Shop|Setup")
	int32 ItemSlotColumn;



	UPROPERTY(BlueprintReadWrite, Category = "Shop", meta = (BindWidget))
	class UNPCShopItemDetails* ItemDetailsWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Shop", meta = (BindWidget))
	class UTextBlock* MoneyText;

	UPROPERTY(BlueprintReadWrite, Category = "Shop", meta = (BindWidget))
	UUniformGridPanel* UniformGrid_Buy;

	UPROPERTY(BlueprintReadWrite, Category = "Shop", meta = (BindWidget))
	UUniformGridPanel* UniformGrid_Sell;

	UPROPERTY(BlueprintReadWrite, Category = "Shop", meta = (BindWidget))
	UUniformGridPanel* UniformGrid_Craft;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Shop")
	TSubclassOf<class UNPCShopItemSlot> ItemSlotWidgetClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Shop")
	EShopItemDisplayedType DisplayedType;
};
