
#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "Item_PotionBase.generated.h"



/**
 * @author 이성수
 * @brief 포션 아이템 베이스 클래스
 * @since 2023-02-23
 */
UCLASS()
class GRADUATEPROJECT_API UItem_PotionBase : public UItemBase
{
	GENERATED_BODY()
	
public:
	UItem_PotionBase();



	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickup")
	TSubclassOf<class UGameplayEffect> EffectClassToApplyToTarget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pickup")
	int32 EffectLevel;
};
