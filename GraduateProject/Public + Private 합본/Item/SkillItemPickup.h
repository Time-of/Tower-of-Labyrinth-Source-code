
#pragma once

#include "CoreMinimal.h"
#include "LevelEvents/InteractableLevelEventBase.h"
#include "SkillItemPickup.generated.h"



/**
 * @author 이성수
 * @brief 스킬 아이템 픽업 액터 클래스입니다.
 * @since 2023-04-25
 */
UCLASS()
class GRADUATEPROJECT_API ASkillItemPickup : public AInteractableLevelEventBase
{
	GENERATED_BODY()

public:
	ASkillItemPickup();



protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnAssignSuccessed();

	void OnInteractUIShown() final;

	void OnInteractUIBeforeHidden() final;

	bool CanInteract_Implementation() override;



	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SkillItemPickup")
	class UItem_CharacterSkillBase* SkillItemToAssign;

	bool bWasAssigned;
};
