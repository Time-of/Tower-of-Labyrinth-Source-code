
#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include <GraduateProject/GraduateProject.h>

#include "NPCShopConfirmInteract.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConfirmButtonClicked, UNPCShopConfirmInteract*, ConfirmWidget, int32, Quantity);



/**
 * @author 이성수
 * @brief 상점 NPC의 확인 창 UI 클래스입니다.
 * @since 2023-02-28
 */
UCLASS()
class GRADUATEPROJECT_API UNPCShopConfirmInteract : public UUserWidget
{
	GENERATED_BODY()

public:
	UNPCShopConfirmInteract(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	/**
	* @brief 확인 창 정보를 업데이트합니다.
	* @param TargetSlot 이 슬롯에 대한 정보를 업데이트합니다.
	*/
	UFUNCTION(BlueprintCallable, Category = "ConfirmInteract")
	void UpdateInfo(class UNPCShopItemSlot* TargetSlot);

	UFUNCTION()
	void OnItemQuantityTextUpdated(const FText& QuantityText);

	UFUNCTION()
	void BroadcastConfirmDelegate();

	UFUNCTION()
	void AddQuantity();

	UFUNCTION()
	void SubQuantity();



	UPROPERTY(BlueprintAssignable, Category = "ConfirmInteract")
	FOnConfirmButtonClicked OnConfirmDelegate;



	UPROPERTY(BlueprintReadWrite, Category = "ConfirmInteract|Confirm", meta = (BindWidget))
	class UButton* Button_InteractConfirm;

	UPROPERTY(BlueprintReadWrite, Category = "ConfirmInteract|Confirm", meta = (BindWidget))
	class UButton* Button_CancelConfirm;



protected:
	/**
	* @brief 수량을 설정하고, 설정된 수량에 따라 여러 작업들을 수행합니다.
	* @details ItemQuantityEditableText의 텍스트를 설정하고, NeedInfo_Money의 필요 가격을 설정하는 등의 작업을 진행합니다.
	*/
	void SetQuantity(int32 NewQuantity);

	void SetCraftNeedItems(const TArray<struct FCraftNeedItem>& NeedItems);

	UFUNCTION()
	void ShowItemDetailsWidget(class UItemBase* Item);

	UFUNCTION()
	void HideItemDetailsWidget(class UItemBase* Item);



	UPROPERTY()
	class UItemBase* ItemToInteract;

	EShopInteractType InteractType;

	UPROPERTY(BlueprintReadWrite, Category = "ConfirmInteract", meta = (BindWidget))
	class UImage* Image_ItemIcon;

	/** @brief 필요한 돈(골드) 위젯 */
	UPROPERTY(BlueprintReadWrite, Category = "ConfirmInteract", meta = (BindWidget))
	class UNPCShopNeedItemInfo* NeedInfo_Money;

	/** @brief 필요한 아이템 목록 버티컬 박스 */
	UPROPERTY(BlueprintReadWrite, Category = "ConfirmInteract", meta = (BindWidget))
	class UVerticalBox* VerticalBox_NeedItems;

	UPROPERTY(BlueprintReadWrite, Category = "ConfirmInteract", meta = (BindWidget))
	class UEditableTextBox* ItemQuantityEditableText;

	UPROPERTY(BlueprintReadWrite, Category = "ConfirmInteract", meta = (BindWidget))
	class UButton* Button_AddQuantity;

	UPROPERTY(BlueprintReadWrite, Category = "ConfirmInteract", meta = (BindWidget))
	class UButton* Button_SubQuantity;

	UPROPERTY(BlueprintReadWrite, Category = "ConfirmInteract", meta = (BindWidget))
	class UTextBlock* InteractConfirmButtonText;

	UPROPERTY(BlueprintReadWrite, Category = "ConfirmInteract", meta = (BindWidget))
	class UNPCShopItemDetails_Simplified* ItemDetailsWidget;

	

	int32 EnteredQuantity;

	int32 MaxQuantity;

	UPROPERTY()
	TSubclassOf<class UNPCShopNeedItemInfo> NeedInfoClass;
};
