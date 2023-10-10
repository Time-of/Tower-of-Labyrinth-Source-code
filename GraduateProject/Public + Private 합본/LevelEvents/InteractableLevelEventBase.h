
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableLevelEventBase.generated.h"



/**
 * @author 이성수
 * @brief 상호작용 가능한 레벨 이벤트 베이스 클래스입니다.
 * @since 2023-04-27
 */
UCLASS()
class GRADUATEPROJECT_API AInteractableLevelEventBase : public AActor
{
	GENERATED_BODY()

public:
	AInteractableLevelEventBase();



protected:
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ShowInteractUI();

	void HideInteractUI();

	void InternalTryInteract();

	/** @brief 상호작용 시도 시 호출 (기본 구현: 여기에서 ShowInteractUI 호출) */
	UFUNCTION(BlueprintNativeEvent)
	void TryInteract();

	/** @brief 상호작용 취소 시 호출 (기본 구현: 여기에서 HideInteractUI 호출) */
	UFUNCTION(BlueprintNativeEvent)
	void CancelInteract();

	/** @brief 상호작용 UI가 보여졌을 때 호출 */
	virtual void OnInteractUIShown();
	
	/** @brief 상호작용 UI가 감춰지기 전에 호출 */
	virtual void OnInteractUIBeforeHidden();

	virtual void BeginPlay() override;

	/** @brief bInternalIsPlayerInRange인 상태에서 상호작용 가능 여부를 결정합니다. */
	UFUNCTION(BlueprintNativeEvent)
	bool CanInteract();

	UFUNCTION(BlueprintCallable)
	void DisableCollision();

	UFUNCTION(BlueprintCallable)
	void SetInteractName(const FString& NewInteractName);



	bool bInternalIsPlayerInRange;

	/**
	* @brief 상호작용 정보 UI에 등장할 '기본' 이름
	* @details 런타임 중 변경 불가 (변경해도 아무 일도 일어나지 않음)
	* @details 대신 SetInteractName()을 이용하세요.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InteractableEvent")
	FString DefaultInteractName;

	/** @brief 상호작용 감지 범위로 사용하는 스피어 콜라이더입니다. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "InteractableEvent")
	class USphereComponent* InteractableRadiusSphere;

	/**
	* @brief InteractableRadiusSphere 범위 내에 플레이어가 있을 때 보이게 되는 위젯 컴포넌트입니다.
	* @see OnSphereBeginOverlap() 참고.
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "InteractableEvent|UI")
	class UWidgetComponent* InteractInfoWidgetComp;

	/** @brief 상호작용 시 보여줄 위젯 클래스. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InteractableEvent|UI")
	TSubclassOf<class UInteractableNPCWidgetBase> WidgetClassToShow;

	UPROPERTY()
	class UInteractableNPCWidgetBase* WidgetToShow_Instance;
};
