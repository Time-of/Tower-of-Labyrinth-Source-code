
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"

#include "PlayerGhostSpectrum.generated.h"



/**
 * @author 이성수
 * @brief 플레이어 캐릭터의 스켈레탈 메시를 복제하여 잔상을 남깁니다.
 * @since 2023-05-28
 */
UCLASS()
class GRADUATEPROJECT_API APlayerGhostSpectrum : public AActor
{
	GENERATED_BODY()

public:
	APlayerGhostSpectrum();

	UFUNCTION(BlueprintCallable, Category = "GhostSpectrum")
	void CopyPose(USkeletalMeshComponent* SkMeshComp, FLinearColor NewColor);



protected:
	UFUNCTION()
	void OnHideTimelineUpdate(float Value);

	UFUNCTION()
	void OnHideTimelineFinished();



	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UPoseableMeshComponent* GhostPoseableMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GhostSpectrum")
	UMaterialInterface* GhostMaterial;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMatArray;

	UPROPERTY(BlueprintReadOnly, Category = "GhostSpectrum")
	class UTimelineComponent* HideTimeline;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GhostSpectrum")
	UCurveFloat* HideTimelineCurveFloat;

	class FOnTimelineFloat HideTimelineUpdate;

	class FOnTimelineEvent HideTimelineFinished;
};
