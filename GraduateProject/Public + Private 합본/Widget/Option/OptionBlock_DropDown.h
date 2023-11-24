
#pragma once

#include "CoreMinimal.h"
#include "Widget/Option/OptionBlockBase.h"
#include "OptionBlock_DropDown.generated.h"

/**
 * @author 이성수
 * @brief 드롭다운 형식의 옵션 블록 위젯 클래스입니다.
 * @since 2023-11-12
 */
UCLASS()
class GRADUATEPROJECT_API UOptionBlock_DropDown : public UOptionBlockBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionBlock", meta = (BindWidget))
	TObjectPtr<class UComboBoxString> DropDownMenu = nullptr;
};
