
#include "Widget/Option/OptionBlock_Progress.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"


UOptionBlock_Progress::UOptionBlock_Progress(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SliderMatAsset(TEXT("/Game/SungSoo_Workspace/Materials/M_UI_Option_ColoredSlider"));
	if (SliderMatAsset.Succeeded())
	{
		SliderMaterialAsset = SliderMatAsset.Object;
	}
}


void UOptionBlock_Progress::NativeConstruct()
{
	Super::NativeConstruct();
	
	SliderMaterial = UMaterialInstanceDynamic::Create(SliderMaterialAsset, this);
	check(SliderMaterial);

	FSliderStyle style = SettingSlider->GetWidgetStyle();
	UWidgetBlueprintLibrary::SetBrushResourceToMaterial(style.NormalBarImage, SliderMaterial);
	UWidgetBlueprintLibrary::SetBrushResourceToMaterial(style.HoveredBarImage, SliderMaterial);
	SettingSlider->SetWidgetStyle(style);
	SettingSlider->OnValueChanged.AddUniqueDynamic(this, &UOptionBlock_Progress::OnSliderValueSet);
}


void UOptionBlock_Progress::NativeDestruct()
{
	Super::NativeDestruct();

	SettingSlider->OnValueChanged.RemoveDynamic(this, &UOptionBlock_Progress::OnSliderValueSet);
}


void UOptionBlock_Progress::OnSliderValueSet(float NewValue)
{
	SliderMaterial->SetScalarParameterValue(TEXT("Percent"), GetSliderPercent());
	SliderValueText->SetText(UKismetTextLibrary::Conv_FloatToText(NewValue, ERoundingMode::HalfToEven, false, true, 1, 10, 0, 1));
}


float UOptionBlock_Progress::GetSliderPercent() const
{
	return SettingSlider->GetValue() / SettingSlider->GetMaxValue();
}
