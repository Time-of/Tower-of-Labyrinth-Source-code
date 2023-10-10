
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractableNPCWidgetBase.generated.h"



/**
 * @author 이성수
 * @brief 상호작용 가능한 NPC들이 사용하는 위젯의 베이스 클래스입니다.
 * @brief (23-03-02) 인벤토리 제작 담당이 이성수로 넘어감에 따라, 상속하여 인벤토리 위젯에도 사용합니다.
 * @since 2023-02-18
 */
UCLASS()
class GRADUATEPROJECT_API UInteractableNPCWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UInteractableNPCWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	/**
	* @brief 뷰포트에 Focus된 동안 Escape 입력 키를 받으면 위젯을 닫을 수 있게끔 오버라이드한 기능입니다.
	*/
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	/**
	* @brief 뷰포트로부터 제거하고 플레이어의 입력 및 카메라를 원래대로 돌려놓습니다.
	*/
	UFUNCTION(BlueprintCallable, Category = "InteractableWidget")
	virtual void RemoveThisFromParent();


};
