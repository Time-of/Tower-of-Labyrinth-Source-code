
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainScreenCreditWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReturnToMainScreenButtonClickedDelegate);


/**
 * @author 이성수
 * @brief 메인 스크린에서 크레딧을 담당하는 위젯입니다.
 * @since 2023-11-11
 */
UCLASS()
class GRADUATEPROJECT_API UMainScreenCreditWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainScreenCreditWidget(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() final;
	void NativeDestruct() final;

	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) final;

	void OnSwitchedToThis();


	FOnReturnToMainScreenButtonClickedDelegate OnReturnToMainScreenButtonClickedDelegate;


protected:
	UFUNCTION()
	void OnReturnButtonClicked();


protected:
	UPROPERTY(BlueprintReadWrite, Category = "TOL|MainScreen|Credit", meta = (BindWidget))
	TObjectPtr<class UButton> ReturnToMainScreenButton = nullptr;
};
