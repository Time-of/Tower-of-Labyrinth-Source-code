
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"

#include "MoneyPickupBase.generated.h"



/**
 * @author 이성수
 * @brief 픽업 가능한 돈 액터
 * @since 2023-02-24
 */
UCLASS()
class GRADUATEPROJECT_API AMoneyPickupBase : public AActor
{
	GENERATED_BODY()

public:
	AMoneyPickupBase();

	void SetMoneyAmount(float NewAmount);



protected:
	virtual void BeginPlay() override;

	/** @brief PickupAnimTimeline이 Update될 때 호출 */
	UFUNCTION()
	void OnPickupAnimTimelineUpdate(float Value);

	/** @brief PickupAnimTimeline이 Finished되었을 때 호출 */
	UFUNCTION()
	void OnPickupAnimTimelineFinished();

	UFUNCTION(BlueprintCallable)
	void StartPickupAnim();



	UPROPERTY(BlueprintReadOnly, Category = "Pickup|Timeline")
	class UTimelineComponent* PickupAnimTimeline;

	/** @brief 실제 PickupAnimTimeline이 진행하게 될 Float 커브 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pickup|Timeline")
	UCurveFloat* PickupAnimCurveFloat;

	class FOnTimelineFloat PickupAnimTimelineUpdate;

	class FOnTimelineEvent PickupAnimTimelineFinished;

	/** @brief 스폰된 후 기다릴 최소 시간 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pickup")
	float MinWaitDuration;

	/** @brief 스폰된 후 기다릴 최대 시간 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pickup")
	float MaxWaitDuration;



	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USphereComponent* CollisionSphereComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* MoneyStaticMeshComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNiagaraSystem* MoneyVFX;

	FVector StartLocation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pickup", meta = (ExposeOnSpawn = "true"))
	float MoneyAmount;
};
