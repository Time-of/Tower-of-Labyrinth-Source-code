
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemPickupBase.generated.h"



/**
 * @author 이성수
 * @brief 바닥에 떨어지고, 픽업이 가능한 아이템 클래스입니다.
 * @since 2023-02-23
 */
UCLASS()
class GRADUATEPROJECT_API AItemPickupBase : public AActor
{
	GENERATED_BODY()

public:
	AItemPickupBase();

	void SetPickupItemInfo(class UItemBase* Item, int32 NewQuantity);



protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyEffectToTarget(AActor* TargetActor);

	/**
	* @brief 아이템을 획득합니다. 블루프린트 재정의가 필요합니다.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void GiveItem(AActor* TargetActor);



	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UStaticMeshComponent* ItemStaticMeshComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USphereComponent* PickupRadiusSphereComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickup")
	class UItemBase* ItemToPickup;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickup")
	int32 ItemQuantity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pickup")
	TSubclassOf<class UGameplayEffect> EffectClassToApplyToTarget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pickup")
	int32 EffectLevel;
};
