
#include "Item/SkillItemPickup.h"

#include "Widget/AssignSkillWidget.h"
#include "Widget/InteractableNPCWidgetBase.h"
#include "Widget/Interface/FloatingWidgetHandler.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"



ASkillItemPickup::ASkillItemPickup() : Super(), 
	bWasAssigned(false)
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultInteractName = TEXT("Acquire Skill Default");

	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultAssignWidgetClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Widget/WBP_AssignSkill");
	if (DefaultAssignWidgetClass.Succeeded())
	{
		WidgetClassToShow = DefaultAssignWidgetClass.Class;
	}
}



void ASkillItemPickup::BeginPlay()
{
	Super::BeginPlay();
	
	
}



void ASkillItemPickup::OnAssignSuccessed()
{
	DisableCollision();

	bWasAssigned = true;

	Destroy();
}



void ASkillItemPickup::OnInteractUIShown()
{
	if (!IsValid(WidgetToShow_Instance)) return;

	if (UAssignSkillWidget* AssignWidget = Cast<UAssignSkillWidget>(WidgetToShow_Instance))
	{
		AssignWidget->SkillItemToAssign = SkillItemToAssign;
		AssignWidget->OnAssignSuccessed.AddUniqueDynamic(this, &ASkillItemPickup::OnAssignSuccessed);
	}
}



void ASkillItemPickup::OnInteractUIBeforeHidden()
{
	if (!IsValid(WidgetToShow_Instance)) return;

	if (UAssignSkillWidget* AssignWidget = Cast<UAssignSkillWidget>(WidgetToShow_Instance))
	{
		AssignWidget->OnAssignSuccessed.RemoveDynamic(this, &ASkillItemPickup::OnAssignSuccessed);
	}
}



bool ASkillItemPickup::CanInteract_Implementation()
{
	return !bWasAssigned;
}
