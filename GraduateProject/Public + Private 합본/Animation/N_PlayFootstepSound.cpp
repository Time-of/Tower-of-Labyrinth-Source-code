
#include "Animation/N_PlayFootstepSound.h"

#include "Player/PlayerCharacterBase.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"



UN_PlayFootstepSound::UN_PlayFootstepSound() : Super(),
	bIsLeftFoot(true),
	Volume(1.0f), Pitch(1.0f)
{
	static ConstructorHelpers::FObjectFinder<USoundBase> SFXCue_Concrete = TEXT("/Game/Assets_Sound/Footstep_Sounds_Pro/Cues_By_Sungsoo/Cue_Footstep_Concrete");
	if (SFXCue_Concrete.Succeeded()) SFX_Concrete = SFXCue_Concrete.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXCue_Dirt = TEXT("/Game/Assets_Sound/Footstep_Sounds_Pro/Cues_By_Sungsoo/Cue_Footstep_Dirt");
	if (SFXCue_Dirt.Succeeded()) SFX_Dirt = SFXCue_Dirt.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXCue_Glass = TEXT("/Game/Assets_Sound/Footstep_Sounds_Pro/Cues_By_Sungsoo/Cue_Footstep_Glass");
	if (SFXCue_Glass.Succeeded()) SFX_Glass = SFXCue_Glass.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXCue_Grass = TEXT("/Game/Assets_Sound/Footstep_Sounds_Pro/Cues_By_Sungsoo/Cue_Footstep_Grass");
	if (SFXCue_Grass.Succeeded()) SFX_Grass = SFXCue_Grass.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXCue_Gravel = TEXT("/Game/Assets_Sound/Footstep_Sounds_Pro/Cues_By_Sungsoo/Cue_Footstep_Gravel");
	if (SFXCue_Gravel.Succeeded()) SFX_Gravel = SFXCue_Gravel.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXCue_Ice = TEXT("/Game/Assets_Sound/Footstep_Sounds_Pro/Cues_By_Sungsoo/Cue_Footstep_Ice");
	if (SFXCue_Ice.Succeeded()) SFX_Ice = SFXCue_Ice.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXCue_Metal = TEXT("/Game/Assets_Sound/Footstep_Sounds_Pro/Cues_By_Sungsoo/Cue_Footstep_Metal");
	if (SFXCue_Metal.Succeeded()) SFX_Metal = SFXCue_Metal.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXCue_Mud = TEXT("/Game/Assets_Sound/Footstep_Sounds_Pro/Cues_By_Sungsoo/Cue_Footstep_Mud");
	if (SFXCue_Mud.Succeeded()) SFX_Mud = SFXCue_Mud.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXCue_Sand = TEXT("/Game/Assets_Sound/Footstep_Sounds_Pro/Cues_By_Sungsoo/Cue_Footstep_Sand");
	if (SFXCue_Sand.Succeeded()) SFX_Sand = SFXCue_Sand.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXCue_Snow = TEXT("/Game/Assets_Sound/Footstep_Sounds_Pro/Cues_By_Sungsoo/Cue_Footstep_Snow");
	if (SFXCue_Snow.Succeeded()) SFX_Snow = SFXCue_Snow.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXCue_Stone = TEXT("/Game/Assets_Sound/Footstep_Sounds_Pro/Cues_By_Sungsoo/Cue_Footstep_Stone");
	if (SFXCue_Stone.Succeeded()) SFX_Stone = SFXCue_Stone.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXCue_Water = TEXT("/Game/Assets_Sound/Footstep_Sounds_Pro/Cues_By_Sungsoo/Cue_Footstep_Water");
	if (SFXCue_Water.Succeeded()) SFX_Water = SFXCue_Water.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXCue_Wood = TEXT("/Game/Assets_Sound/Footstep_Sounds_Pro/Cues_By_Sungsoo/Cue_Footstep_Wood");
	if (SFXCue_Wood.Succeeded()) SFX_Wood = SFXCue_Wood.Object;


	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> VFXObj_Dirt = TEXT("/Game/Assets/A_Surface_Footstep/Niagara_FX/ParticleSystems/PSN_Dirt_Surface");
	if (VFXObj_Dirt.Succeeded()) VFX_Dirt = VFXObj_Dirt.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> VFXObj_General = TEXT("/Game/Assets/A_Surface_Footstep/Niagara_FX/ParticleSystems/PSN_General2_Surface");
	if (VFXObj_General.Succeeded()) VFX_General = VFXObj_General.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> VFXObj_Grass = TEXT("/Game/Assets/A_Surface_Footstep/Niagara_FX/ParticleSystems/PSN_Grass_Surface");
	if (VFXObj_Grass.Succeeded()) VFX_Grass = VFXObj_Grass.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> VFXObj_Snow = TEXT("/Game/Assets/A_Surface_Footstep/Niagara_FX/ParticleSystems/PSN_SnowLight_Surface");
	if (VFXObj_Snow.Succeeded()) VFX_Snow = VFXObj_Snow.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> VFXObj_Ice = TEXT("/Game/Assets/A_Surface_Footstep/Niagara_FX/ParticleSystems/PSN_Ice_Surface");
	if (VFXObj_Ice.Succeeded()) VFX_Ice = VFXObj_Ice.Object;
}



void UN_PlayFootstepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!GWorld->HasBegunPlay()) return;

	const FVector StartLocation = MeshComp->GetOwner()->GetActorLocation();
	const TArray<AActor*> IgnoreActors { MeshComp->GetOwner() };
	FHitResult HitResult;
	
	// TraceTypeQuery3은 FootstepTrace 채널!
	bool bHit = UKismetSystemLibrary::LineTraceSingle(MeshComp->GetWorld(), StartLocation, StartLocation + FVector::DownVector * 100.0f,
		ETraceTypeQuery::TraceTypeQuery3, false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);

	USoundBase* SelectedSound = nullptr;
	UNiagaraSystem* SelectedVfx = nullptr;

	if (bHit)
	{
		switch (UGameplayStatics::GetSurfaceType(HitResult))
		{
			// 콘크리트 (Concrete)
			case EPhysicalSurface::SurfaceType1:
				SelectedSound = SFX_Concrete;
				SelectedVfx = VFX_General;
				break;
			// 흙 (Dirt)
			case EPhysicalSurface::SurfaceType2:
				SelectedSound = SFX_Dirt;
				SelectedVfx = VFX_Dirt;
				break;
			// 유리 (Glass)
			case EPhysicalSurface::SurfaceType3:
				SelectedSound = SFX_Glass;
				SelectedVfx = VFX_General;
				break;
			// 잔디 (Grass)
			case EPhysicalSurface::SurfaceType4:
				SelectedSound = SFX_Grass;
				SelectedVfx = VFX_Grass;
				break;
			// 자갈 (Gravel)
			case EPhysicalSurface::SurfaceType5:
				SelectedSound = SFX_Gravel;
				SelectedVfx = VFX_General;
				break;
			// 얼음 (Ice)
			case EPhysicalSurface::SurfaceType6:
				SelectedSound = SFX_Ice;
				SelectedVfx = VFX_Ice;
				break;
			// 금속 (Metal)
			case EPhysicalSurface::SurfaceType7:
				SelectedSound = SFX_Metal;
				SelectedVfx = VFX_General;
				break;
			// 진흙 (Mud)
			case EPhysicalSurface::SurfaceType8:
				SelectedSound = SFX_Mud;
				SelectedVfx = VFX_General;
				break;
			// 모래 (Sand)
			case EPhysicalSurface::SurfaceType9:
				SelectedSound = SFX_Sand;
				SelectedVfx = VFX_General;
				break;
			// 눈 (Snow)
			case EPhysicalSurface::SurfaceType10:
				SelectedSound = SFX_Snow;
				SelectedVfx = VFX_Snow;
				break;
			// 돌 (Stone)
			case EPhysicalSurface::SurfaceType11:
				SelectedSound = SFX_Stone;
				SelectedVfx = VFX_General;
				break;
			// 물 (Water)
			case EPhysicalSurface::SurfaceType12:
				SelectedSound = SFX_Water;
				break;
			// 나무 (Wood)
			case EPhysicalSurface::SurfaceType13:
				SelectedSound = SFX_Wood;
				SelectedVfx = VFX_General;
				break;
			case EPhysicalSurface::SurfaceType_Default:
			default:
				break;
		}

		UGameplayStatics::SpawnSoundAtLocation(MeshComp->GetWorld(), SelectedSound, HitResult.Location,
			FRotator::ZeroRotator, Volume, Pitch);


#if WITH_EDITOR
		if (!IsValid(SelectedVfx))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("N_PlayFootstepSound: 지면 유효하지 않음!"));
		}
#endif
		

		if (APlayerCharacterBase* PC = Cast<APlayerCharacterBase>(MeshComp->GetOwner()))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				PC,
				SelectedVfx,
				MeshComp->GetSocketLocation(bIsLeftFoot ? TEXT("LeftFootSocket") : TEXT("RightFootSocket")),
				FRotator::ZeroRotator,
				FVector::OneVector * 2,
				true,
				true,
				ENCPoolMethod::AutoRelease
				);
		}
		else
		{
			#if WITH_EDITOR
			GEngine->AddOnScreenDebugMessage(-1, 13.0f, FColor::Red, TEXT("N_PlayFootstepSound: PCRef 유효하지 않음"));
			#endif
		}
	}

	Super::Notify(MeshComp, Animation, EventReference);
}



FString UN_PlayFootstepSound::GetNotifyName_Implementation() const
{
	return TEXT("발자국 소리 재생");
}
