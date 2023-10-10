
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BossPlantHandler.generated.h"

UINTERFACE(MinimalAPI)
class UBossPlantHandler : public UInterface
{
	GENERATED_BODY()
};



/**
 * @author 이성수
 * @brief 식물 보스를 컨트롤하기 위한 인터페이스 클래스입니다.
 * @details 2023-05-25 폐기됨
 * @since 2023-03-28
 */
class GRADUATEPROJECT_API IBossPlantHandler
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interface|Boss|Plant")
	TArray<FVector> GetRootSpawnLocations() const;
};
