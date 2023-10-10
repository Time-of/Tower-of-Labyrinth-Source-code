
#include "NPC/InteractableNPC.h"

#include "Widget/InteractableNPCWidgetBase.h"
#include "Widget/Interface/FloatingWidgetHandler.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"



AInteractableNPC::AInteractableNPC() : Super(),
	bCanInteract(false),
	NPCName(TEXT("DefaultNPCName")),
	FocusCamera(nullptr),
	InteractableRadiusSphere(nullptr),
	InteractInfoWidgetComp(nullptr),
	WidgetClassToShow(nullptr),
	WidgetToShow_Instance(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FocusCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Focus Camera"));
	FocusCamera->SetupAttachment(GetCapsuleComponent());
	FocusCamera->SetRelativeLocation(FVector(150.0f, 0.0f, 40.0f));
	FocusCamera->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	InteractableRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Interactable Radius Sphere"));
	InteractableRadiusSphere->SetupAttachment(GetCapsuleComponent());
	InteractableRadiusSphere->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	InteractableRadiusSphere->SetSphereRadius(225.0f);

	InteractInfoWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractInfo Widget Comp"));
	InteractInfoWidgetComp->SetupAttachment(GetCapsuleComponent());
	InteractInfoWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	InteractInfoWidgetComp->SetDrawSize(FVector2D(300.0f, 200.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultInteractInfoWidgetClass = TEXT("/Game/SungSoo_Workspace/Blueprints/Widget/NPC/WBP_NPC_InteractInfo");

	if (DefaultInteractInfoWidgetClass.Succeeded())
	{
		InteractInfoWidgetComp->SetWidgetClass(DefaultInteractInfoWidgetClass.Class);
	}
}



void AInteractableNPC::BeginPlay()
{
	Super::BeginPlay();

	InteractableRadiusSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &AInteractableNPC::OnSphereBeginOverlap);
	InteractableRadiusSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &AInteractableNPC::OnSphereEndOverlap);


	if (IsValid(InteractInfoWidgetComp->GetUserWidgetObject()) && InteractInfoWidgetComp->GetUserWidgetObject()->Implements<UFloatingWidgetHandler>())
	{
		IFloatingWidgetHandler::Execute_SetWidgetText(InteractInfoWidgetComp->GetUserWidgetObject(), FText::FromString(NPCName));
		InteractInfoWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Collapsed);
	}


	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PC != nullptr && IsValid(PC->InputComponent))
	{
		PC->InputComponent->BindAction(TEXT("Interact"), IE_Released, this, &AInteractableNPC::ShowInteractUI);
	}
}



void AInteractableNPC::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		bCanInteract = true;
		InteractInfoWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}



void AInteractableNPC::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		bCanInteract = false;
		InteractInfoWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Collapsed);
		
		HideInteractUI();
	}
}



void AInteractableNPC::ShowInteractUI()
{
	if (!bCanInteract || !IsValid(WidgetClassToShow)) return;

	//UE_LOG(LogTemp, Log, TEXT("%s: %s"), *GetName(), TEXT(__FUNCTION__));

	if (!IsValid(WidgetToShow_Instance))
	{
		WidgetToShow_Instance = CreateWidget<UInteractableNPCWidgetBase>(GetWorld(), WidgetClassToShow);
	}
	else if (WidgetToShow_Instance->IsInViewport()) return;

	WidgetToShow_Instance->AddToViewport(11);

	//UE_LOG(LogTemp, Log, TEXT("위젯 생성 크래시 오류? : %s"), IsValid(WidgetToShow_Instance) ? *WidgetToShow_Instance->GetName() : *FString(TEXT("위젯 없음")));

	APlayerController* PlayerControllerRef = GWorld->GetFirstPlayerController();

	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->SetShowMouseCursor(true);
		PlayerControllerRef->SetViewTargetWithBlend(this, 0.2f, VTBlend_Cubic, 3.0f);

		FInputModeUIOnly UIOnlyInputMode;
		UIOnlyInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerControllerRef->SetInputMode(UIOnlyInputMode);

		PostShowInteractUI();
	}
}



void AInteractableNPC::HideInteractUI()
{
	if (!IsValid(WidgetToShow_Instance)) return;

	WidgetToShow_Instance->RemoveThisFromParent();

	PostHideInteractUI();
}
