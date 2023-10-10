
#include "LevelEvents/InteractableLevelEventBase.h"

#include "Widget/InteractableNPCWidgetBase.h"
#include "Widget/Interface/FloatingWidgetHandler.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"



AInteractableLevelEventBase::AInteractableLevelEventBase() : Super(),
	bInternalIsPlayerInRange(false),
	DefaultInteractName(TEXT("DefaultInteractName")),
	InteractableRadiusSphere(nullptr),
	InteractInfoWidgetComp(nullptr),
	WidgetClassToShow(nullptr),
	WidgetToShow_Instance(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

	InteractableRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Interactable Radius Sphere"));
	//InteractableRadiusSphere->SetupAttachment(GetRootComponent());
	InteractableRadiusSphere->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	InteractableRadiusSphere->SetSphereRadius(225.0f);
	SetRootComponent(InteractableRadiusSphere);

	InteractInfoWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractInfo Widget Comp"));
	InteractInfoWidgetComp->SetupAttachment(InteractableRadiusSphere);
	InteractInfoWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	InteractInfoWidgetComp->SetDrawSize(FVector2D(300.0f, 200.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultInteractInfoWidgetClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Widget/NPC/WBP_NPC_InteractInfo");
	if (DefaultInteractInfoWidgetClass.Succeeded())
	{
		InteractInfoWidgetComp->SetWidgetClass(DefaultInteractInfoWidgetClass.Class);
	}
}



void AInteractableLevelEventBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		bInternalIsPlayerInRange = true;
		InteractInfoWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}



void AInteractableLevelEventBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		bInternalIsPlayerInRange = false;

		if (IsValid(InteractInfoWidgetComp))
			InteractInfoWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Collapsed);

		CancelInteract();
	}
}



void AInteractableLevelEventBase::ShowInteractUI()
{
	if (!IsValid(WidgetClassToShow))
		return;

	if (!IsValid(WidgetToShow_Instance))
	{
		WidgetToShow_Instance = CreateWidget<UInteractableNPCWidgetBase>(GetWorld(), WidgetClassToShow);
	}
	else if (WidgetToShow_Instance->IsInViewport()) return;

	WidgetToShow_Instance->AddToViewport(11);
	
	OnInteractUIShown();

	UWorld* WorldRef = GetWorld();
	APlayerController* PlayerControllerRef = IsValid(WorldRef) ? WorldRef->GetFirstPlayerController() : nullptr;

	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->SetShowMouseCursor(true);

		FInputModeUIOnly UIOnlyInputMode;
		UIOnlyInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerControllerRef->SetInputMode(UIOnlyInputMode);
	}
}



void AInteractableLevelEventBase::HideInteractUI()
{
	if (!IsValid(WidgetToShow_Instance)) return;

	OnInteractUIBeforeHidden();

	WidgetToShow_Instance->RemoveThisFromParent();
}



void AInteractableLevelEventBase::InternalTryInteract()
{
	if (!bInternalIsPlayerInRange || !CanInteract()) return;

	TryInteract();
}



void AInteractableLevelEventBase::TryInteract_Implementation()
{
	ShowInteractUI();
}



void AInteractableLevelEventBase::CancelInteract_Implementation()
{
	HideInteractUI();
}



void AInteractableLevelEventBase::OnInteractUIShown()
{
}



void AInteractableLevelEventBase::OnInteractUIBeforeHidden()
{
}



void AInteractableLevelEventBase::BeginPlay()
{
	Super::BeginPlay();

	InteractableRadiusSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &AInteractableLevelEventBase::OnSphereBeginOverlap);
	InteractableRadiusSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &AInteractableLevelEventBase::OnSphereEndOverlap);


	SetInteractName(DefaultInteractName);


	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC != nullptr && IsValid(PC->InputComponent))
	{
		PC->InputComponent->BindAction(TEXT("Interact"), IE_Released, this, &AInteractableLevelEventBase::InternalTryInteract);
	}
}



bool AInteractableLevelEventBase::CanInteract_Implementation()
{
	return true;
}



void AInteractableLevelEventBase::DisableCollision()
{
	InteractableRadiusSphere->SetCollisionProfileName(TEXT("NoCollision"));

	CancelInteract();

	if (IsValid(InteractInfoWidgetComp))
		InteractInfoWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Collapsed);

	bInternalIsPlayerInRange = false;
}



void AInteractableLevelEventBase::SetInteractName(const FString& NewInteractName)
{
	if (IsValid(InteractInfoWidgetComp->GetUserWidgetObject()) && InteractInfoWidgetComp->GetUserWidgetObject()->Implements<UFloatingWidgetHandler>())
	{
		IFloatingWidgetHandler::Execute_SetWidgetText(InteractInfoWidgetComp->GetUserWidgetObject(), FText::FromString(NewInteractName));
		InteractInfoWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Collapsed);
	}
}

