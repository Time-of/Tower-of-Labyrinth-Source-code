
#include "Player/PlayerGhostSpectrum.h"

#include "Components/PoseableMeshComponent.h"



APlayerGhostSpectrum::APlayerGhostSpectrum() : Super(),
	GhostPoseableMesh(nullptr),
	GhostMaterial(nullptr),
	DynamicMatArray(),
	HideTimeline(nullptr),
	HideTimelineCurveFloat(nullptr),
	HideTimelineUpdate(),
	HideTimelineFinished()
{
	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 7.0f;

	GhostPoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("GhostMesh"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFloatObject(TEXT("/Game/SungSoo_Workspace/DataTables/CurveFloat_PlayerGhostSpectrum"));
	if (CurveFloatObject.Succeeded()) HideTimelineCurveFloat = CurveFloatObject.Object;

	HideTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("HideTimelineComp"));
	HideTimelineUpdate.BindUFunction(this, TEXT("OnHideTimelineUpdate"));
	HideTimelineFinished.BindUFunction(this, TEXT("OnHideTimelineFinished"));
	HideTimeline->AddInterpFloat(HideTimelineCurveFloat, HideTimelineUpdate);
	HideTimeline->SetTimelineFinishedFunc(HideTimelineFinished);
}



void APlayerGhostSpectrum::CopyPose(USkeletalMeshComponent* SkMeshComp, FLinearColor NewColor)
{
	if (!IsValid(SkMeshComp) || !IsValid(GhostMaterial)) return;

	GhostPoseableMesh->CopyPoseFromSkeletalComponent(SkMeshComp);
	int32 Length = GhostPoseableMesh->GetMaterials().Num();

	for (int32 i = 0; i < Length; ++i)
	{
		UMaterialInstanceDynamic* CreatedDynamic = UMaterialInstanceDynamic::Create(GhostMaterial, GhostPoseableMesh);
		CreatedDynamic->SetVectorParameterValue(TEXT("Color"), NewColor);
		GhostPoseableMesh->SetMaterial(i, CreatedDynamic);
	
		DynamicMatArray.Add(CreatedDynamic);
	}

	HideTimeline->PlayFromStart();
}



void APlayerGhostSpectrum::OnHideTimelineUpdate(float Value)
{
	for (auto& Mat : DynamicMatArray)
	{
		Mat->SetScalarParameterValue(TEXT("Opacity"), Value);
	}
}



void APlayerGhostSpectrum::OnHideTimelineFinished()
{
	Destroy();
}
