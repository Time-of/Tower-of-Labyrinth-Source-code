
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractableNPC.generated.h"



/**
 * @author 이성수
 * @brief 상호작용 가능한 NPC입니다.
 * @details 플레이어 캐릭터가 가까이 오면 상호작용 메뉴가 등장하고, 정해진 UI에 접근할 수 있습니다.
 * @since 2023-02-17
 */
UCLASS()
class GRADUATEPROJECT_API AInteractableNPC : public ACharacter
{
	GENERATED_BODY()

public:
	AInteractableNPC();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ShowInteractUI();

	void HideInteractUI();

	UFUNCTION(BlueprintImplementableEvent)
	void PostShowInteractUI();

	UFUNCTION(BlueprintImplementableEvent)
	void PostHideInteractUI();

	bool bCanInteract;

	/**
	* @brief 이 NPC의 이름입니다.
	* @details 런타임 중 변경 불가 (변경해도 아무 일도 일어나지 않음)
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "NPC")
	FString NPCName;

	/**
	* @brief 상호작용 중 사용될 카메라입니다.
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "NPC|Interact")
	class UCameraComponent* FocusCamera;

	/**
	* @brief 상호작용 감지 범위로 사용하는 스피어 콜라이더입니다.
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "NPC|Interact")
	class USphereComponent* InteractableRadiusSphere;

	/**
	* @brief InteractableRadiusSphere 범위 내에 플레이어가 있을 때 보이게 되는 위젯 컴포넌트입니다.
	* @see OnSphereBeginOverlap() 참고.
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "NPC|Interact|UI")
	class UWidgetComponent* InteractInfoWidgetComp;

	/**
	* @brief 상호작용 시 보여줄 위젯 클래스.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "NPC|Interact|UI")
	TSubclassOf<class UInteractableNPCWidgetBase> WidgetClassToShow;

	// UPROPERTY를 붙이지 않으면 GC 대상에 포함되기 때문에, GC 이후 쓰레기 값이 들어가 크래시 발생시킴
	UPROPERTY()
	UInteractableNPCWidgetBase* WidgetToShow_Instance;
};
