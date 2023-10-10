
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySkillDetailsWidget.generated.h"



/**
 * @author 이성수
 * @brief 인벤토리 스킬 디테일 위젯 클래스입니다.
 * @since 2023-03-03
 */
UCLASS()
class GRADUATEPROJECT_API UInventorySkillDetailsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventorySkillDetailsWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void UpdateSkillDetails(class UItem_CharacterSkillBase* SkillItem);



protected:
	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Skill", meta = (BindWidget))
	class UTextBlock* SkillNameText;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Skill", meta = (BindWidget))
	class URichTextBlock* SkillDescriptionText;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Skill", meta = (BindWidget))
	class UTextBlock* SkillCostText;
};
