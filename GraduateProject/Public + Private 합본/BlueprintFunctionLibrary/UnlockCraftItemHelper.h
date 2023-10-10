
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnlockCraftItemHelper.generated.h"



/**
 * @author 이성수
 * @brief 제작 아이템 해금을 도와주는 블루프린트 함수 라이브러리 클래스입니다.
 * @since 2023-04-05
 */
UCLASS()
class GRADUATEPROJECT_API UUnlockCraftItemHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UnlockCraftItemHelper")
	static void UnlockCraftItem(AActor* WorldContext, const FPrimaryAssetId& Item);
};
