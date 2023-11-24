
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionBlockBase.generated.h"

/**
 * @author 이성수
 * @brief 옵션 블록 베이스 위젯 클래스입니다.
 * @details 텍스트와 공백으로 구성되어 있으며, 상속하여 상호작용을 사용합니다.
 * @since 2023-11-11
 */
UCLASS()
class GRADUATEPROJECT_API UOptionBlockBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UOptionBlockBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "TOL|OptionWidget|OptionBlock")
	void K2_SetOptionNameText(FText NewText);

	void SetOptionNameText(const FText& NewText);


protected:
	UPROPERTY(BlueprintReadWrite, Category = "TOL|OptionWidget|OptionBlock", meta = (BindWidget))
	TObjectPtr<class UTextBlock> OptionNameText = nullptr;
};
