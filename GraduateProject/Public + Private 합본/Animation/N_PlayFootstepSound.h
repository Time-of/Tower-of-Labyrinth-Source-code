
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "N_PlayFootstepSound.generated.h"



/**
 * @author 이성수
 * @brief 캐릭터 아래의 피지컬 머티리얼에 따라 발소리를 다르게 내는 노티파이 클래스입니다.
 * @since 2023-02-14
 */
UCLASS()
class GRADUATEPROJECT_API UN_PlayFootstepSound : public UAnimNotify
{
	GENERATED_BODY()

public:
	UN_PlayFootstepSound();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;



protected:
	//class APlayerCharacterBase* GetPCRef(USkeletalMeshComponent* MeshComp) const;



	//UPROPERTY(Transient)
	//class APlayerCharacterBase* PCRef;

	UPROPERTY(EditAnywhere)
	bool bIsLeftFoot;



	UPROPERTY(EditAnywhere)
	float Volume;

	UPROPERTY(EditAnywhere)
	float Pitch;



	UPROPERTY()
	USoundBase* SFX_Concrete;

	UPROPERTY()
	USoundBase* SFX_Dirt;

	UPROPERTY()
	USoundBase* SFX_Glass;

	UPROPERTY()
	USoundBase* SFX_Grass;

	UPROPERTY()
	USoundBase* SFX_Gravel;

	UPROPERTY()
	USoundBase* SFX_Ice;

	UPROPERTY()
	USoundBase* SFX_Metal;

	UPROPERTY()
	USoundBase* SFX_Mud;

	UPROPERTY()
	USoundBase* SFX_Sand;

	UPROPERTY()
	USoundBase* SFX_Snow;

	UPROPERTY()
	USoundBase* SFX_Stone;

	UPROPERTY()
	USoundBase* SFX_Water;

	UPROPERTY()
	USoundBase* SFX_Wood;

	UPROPERTY()
	class UNiagaraSystem* VFX_Dirt;

	UPROPERTY()
	class UNiagaraSystem* VFX_General;

	UPROPERTY()
	class UNiagaraSystem* VFX_Grass;

	UPROPERTY()
	class UNiagaraSystem* VFX_Snow;

	UPROPERTY()
	class UNiagaraSystem* VFX_Ice;
};
