
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoadingGameMode.generated.h"



/**
 * @author 이성수
 * @brief 심리스 트래블을 사용한 로딩 스크린 구현 게임모드입니다.
 * @since 2023-05-24
 */
UCLASS()
class GRADUATEPROJECT_API ALoadingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALoadingGameMode();

	void InitSeamlessTravelPlayer(AController* NewController) override;

	void PostSeamlessTravel() override;

	void StartToLeaveMap() override;
};
