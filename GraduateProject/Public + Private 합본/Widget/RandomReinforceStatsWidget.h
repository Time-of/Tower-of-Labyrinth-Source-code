
#pragma once

#include "CoreMinimal.h"
#include "Widget/InteractableNPCWidgetBase.h"
#include "RandomReinforceStatsWidget.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmSuccessedDelegate);



/**
 * @author 이성수
 * @brief 무작위 스탯 강화 이펙트를 부여하기 위해 사용되는 위젯 클래스입니다.
 * @since 2023-04-27
 */
UCLASS()
class GRADUATEPROJECT_API URandomReinforceStatsWidget : public UInteractableNPCWidgetBase
{
	GENERATED_BODY()

public:
	URandomReinforceStatsWidget(const FObjectInitializer& ObjectInitializer);



	FOnConfirmSuccessedDelegate OnConfirmSuccessed;



protected:
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;



	UPROPERTY(BlueprintReadWrite, Category = "RandomReinforceStatsWidget", meta = (BindWidget))
	class UButton* ConfirmButton;



private:
	UFUNCTION()
	void InternalOnConfirmButtonClicked();
};
