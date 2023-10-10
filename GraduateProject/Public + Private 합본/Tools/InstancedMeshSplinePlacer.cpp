
#include "Tools/InstancedMeshSplinePlacer.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"


AInstancedMeshSplinePlacer::AInstancedMeshSplinePlacer() :
	InstancedMeshComp(nullptr),
	SplineComp(nullptr),
	MeshPlaceInterval(225.0f),
	ScaleModifier(1.0f),
	bDrawMeshInEditor(true)
{
	PrimaryActorTick.bCanEverTick = false;

	InstancedMeshComp = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("InstancedMeshComp"));
	SetRootComponent(InstancedMeshComp);

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	SplineComp->SetupAttachment(InstancedMeshComp);
}


void AInstancedMeshSplinePlacer::OnConstruction(const FTransform& Transform)
{
	SetActorRelativeScale3D(FVector::OneVector);
	InstancedMeshComp->SetRelativeScale3D(FVector::OneVector * ScaleModifier);

	RebuildSplineAndInstancedSMs(false);
}


void AInstancedMeshSplinePlacer::BeginPlay()
{
	Super::BeginPlay();

	RebuildSplineAndInstancedSMs(true);
}


void AInstancedMeshSplinePlacer::RebuildSplineAndInstancedSMs(bool bForceDrawMeshes)
{
	SplineComp->ClearSplinePoints();
	InstancedMeshComp->ClearInstances();

	int i = 0;
	for (const FVector& Point : SplinePoints)
	{
		SplineComp->AddSplinePointAtIndex(Point, i++, ESplineCoordinateSpace::Local);
	}


	if (!bForceDrawMeshes && !bDrawMeshInEditor) return;


	float SplineLength = SplineComp->GetSplineLength();
	if (MeshPlaceInterval <= 0.0f || SplineLength <= 0.0f) return;


	int truncated = FMath::FloorToInt32(SplineLength / MeshPlaceInterval);
	for (i = 0; i < truncated; ++i)
	{
		InstancedMeshComp->AddInstance(
			SplineComp->GetTransformAtTime((i * MeshPlaceInterval) / SplineLength, ESplineCoordinateSpace::Local, true, true)
			);
	}


#if WITH_EDITOR
	if (SplinePoints.Num() >= 10 && IsValid(GEngine))
	{
		// 너무 많을 경우 경고 메시지 띄우기
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("%s: 혹시 너무 넓은 범위에 걸쳐있다면, 별로 최적화에 도움이 되지 않습니다! 당장 해당 액터를 둘로 나누어 사용하는 것을 고려하세요."), *GetName()));
	}
#endif
}
