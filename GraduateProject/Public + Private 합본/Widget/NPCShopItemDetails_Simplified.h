
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCShopItemDetails_Simplified.generated.h"



/**
 * @author 이성수
 * @brief 간략화된 아이템 디테일 창입니다.
 * @since 2023-04-14
 */
UCLASS()
class GRADUATEPROJECT_API UNPCShopItemDetails_Simplified : public UUserWidget
{
	GENERATED_BODY()

public:
	UNPCShopItemDetails_Simplified(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	/**
	* @brief 아이템 정보를 업데이트합니다.
	* @param Item 아이템 정보
	*/
	UFUNCTION(BlueprintCallable, Category = "ItemDetails")
	virtual void UpdateItem(const class UItemBase* Item);



protected:
	void SetAdditionalDescriptionText(const class UItemBase* Item);



	UPROPERTY(BlueprintReadWrite, Category = "ItemDetails", meta = (BindWidget))
	class UTextBlock* ItemNameText;

	UPROPERTY(BlueprintReadWrite, Category = "ItemDetails", meta = (BindWidget))
	class UTextBlock* ItemTypeText;

	UPROPERTY(BlueprintReadWrite, Category = "ItemDetails", meta = (BindWidget))
	class UTextBlock* AdditionalDescriptionText;

	UPROPERTY(BlueprintReadWrite, Category = "ItemDetails", meta = (BindWidget))
	class URichTextBlock* ItemDescriptionText;
};
