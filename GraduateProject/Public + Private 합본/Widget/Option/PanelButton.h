
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PanelButton.generated.h"

/**
 * @author 이성수
 * @brief 블루프린트에서 이것저것 꾸미고, 버튼을 C++로 노출시키기 위한 클래스입니다.
 * @since 2023-11-12
 */
UCLASS()
class GRADUATEPROJECT_API UPanelButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "TOL|Option")
	void K2_SetButtonText(FText NewText);
	
	UFUNCTION(BlueprintCallable, Category = "TOL|Option")
	void K2_SetIconImage(UTexture2D* NewTexture);



	UPROPERTY(BlueprintReadWrite, Category = "TOL|Option", meta = (BindWidget))
	TObjectPtr<class UButton> Button_ = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "TOL|Option", meta = (BindWidget))
	TObjectPtr<class UTextBlock> ButtonText = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Category = "TOL|Option", meta = (BindWidget))
	TObjectPtr<class UImage> Icon = nullptr;
};
