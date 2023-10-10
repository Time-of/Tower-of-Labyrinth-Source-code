
#pragma once

#include "CoreMinimal.h"
#include "Widget/InteractableNPCWidgetBase.h"
#include "NPCTrainingWidget.generated.h"



/**
 * @author 이성수
 * @brief 훈련장 NPC가 사용할 위젯 클래스입니다.
 * @since 2023-02-18
 */
UCLASS()
class GRADUATEPROJECT_API UNPCTrainingWidget : public UInteractableNPCWidgetBase
{
	GENERATED_BODY()

public:
	UNPCTrainingWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;



protected:
	/** @brief 돈 텍스트를 업데이트합니다. */
	UFUNCTION()
	void UpdateMoneyText(float NewAmount);

	/**
	* @brief SelectedAttributeType을 설정하고, NONE이 아니라면 ExecuteButton을 활성화합니다.
	* @param TypeToSelect 선택할 강화 타입
	* @param[out] OutPrice 선택한 강화 타입을 강화하는 데 필요한 비용
	*/
	UFUNCTION(BlueprintCallable, Category = "InteractableWidget|Training")
	void SelectTrainingAttributeType(ETrainingAttributeType TypeToSelect);

	/**
	* @brief SelectedAttributeType에 따라 강화를 실제로 수행합니다.
	* @details 블루프린트에서 강화 실행 버튼의 OnClicked에 달아 놓을 것.
	* @return 강화 성공 시 true, 실패 시 false
	*/
	UFUNCTION(BlueprintCallable, Category = "InteractableWidget|Training")
	bool ExecuteTrainingReinforce();

	/**
	* @brief 선택된 어트리뷰트의 가격을 가져옵니다.
	* @return SelectedAttributeType의 현재 레벨에 따른 가격 값
	*/
	UFUNCTION(BlueprintCallable, Category = "InteractableWidget|Training")
	float GetSelectedAttributeTrainingPrice() const;

	UFUNCTION(BlueprintCallable, Category = "InteractableWidget|Training")
	void SetReinforceDescriptionText();



	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "InteractableWidget|Training")
	ETrainingAttributeType SelectedAttributeType;

	UPROPERTY(BlueprintReadWrite, Category = "InteractableWidget|Training", meta = (BindWidget))
	class UButton* ExecuteReinforceButton;

	UPROPERTY(BlueprintReadWrite, Category = "InteractableWidget|Training", meta = (BindWidget))
	class UTextBlock* MoneyText;

	UPROPERTY(BlueprintReadWrite, Category = "InteractableWidget|Training", meta = (BindWidget))
	class UTextBlock* ReinforceDescriptionText;



	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InteractableWidget|Training|Setup")
	UCurveTable* TrainingReinforceCurveTable;
};
