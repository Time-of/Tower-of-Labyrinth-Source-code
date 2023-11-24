
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionPage.generated.h"

/**
 * @author 이성수
 * @brief 옵션 페이지 위젯입니다. OptionBlockBase 위젯들을 담고 있습니다.
 * @details MainScreenOptionWidget은 OptionPage들을 담는 형태로 구성되어 있습니다.
 * @since 2023-11-11
 */
UCLASS()
class GRADUATEPROJECT_API UOptionPage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UOptionPage(const FObjectInitializer& ObjectInitializer);
};
