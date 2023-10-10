
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InstancedMeshSplinePlacer.generated.h"



/**
* @author 이성수
* @brief 인스턴스드 스태틱 메시를 스플라인 형태로 쉽게 배치할 수 있도록 도와주는 레벨 디자인 툴입니다.
* @details 주의!!! 하나의 HISM은 오클루전 컬링 범위 내에 인스턴스가 하나라도 있다면 컬링되지 않으므로,
* @details   HISM을 여러 개로 나누어 사용할 것!
* @since 2023-09-25
*/
UCLASS()
class GRADUATEPROJECT_API AInstancedMeshSplinePlacer : public AActor
{
	GENERATED_BODY()

public:
	AInstancedMeshSplinePlacer();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;


protected:
	UFUNCTION(BlueprintCallable, Category = "TOL|Tools|InstancedMeshSplinePlacer")
	void RebuildSplineAndInstancedSMs(bool bForceDrawMeshes);


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TOL|Tools|InstancedMeshSplinePlacer")
	class UHierarchicalInstancedStaticMeshComponent* InstancedMeshComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TOL|Tools|InstancedMeshSplinePlacer")
	class USplineComponent* SplineComp;
	
	/** @brief 메시가 배치될 간격 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TOL|Tools|InstancedMeshSplinePlacer")
	float MeshPlaceInterval;

	/** @brief 스플라인 곡선(Spline)의 각 지점 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TOL|Tools|InstancedMeshSplinePlacer", meta = (MakeEditWidget = true))
	TArray<FVector> SplinePoints;

	/** @brief 메시 스케일 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TOL|Tools|InstancedMeshSplinePlacer")
	float ScaleModifier;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TOL|Tools|InstancedMeshSplinePlacer")
	bool bDrawMeshInEditor;
};
