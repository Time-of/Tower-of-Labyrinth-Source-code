
#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include <GraduateProject/GraduateProject.h>
#include "GameplayEffect.h"

#include "GISS_TemporaryPlayerData.generated.h"



USTRUCT(BlueprintType)
struct GRADUATEPROJECT_API FReinforceEffectData
{
	GENERATED_BODY()

public:
	FReinforceEffectData() : ReinforceEffect(nullptr),
		ReinforceAmount(0.0f) {}

	FReinforceEffectData(const TSubclassOf<UGameplayEffect>& NewReinforceEffect, float NewReinforceAmount) : ReinforceEffect(NewReinforceEffect),
		ReinforceAmount(NewReinforceAmount)
	{

	}

	/** @brief 적용할 이펙트 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> ReinforceEffect;

	/** @brief 적용할 이펙트에 사용될 수치 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ReinforceAmount;
};



/**
 * @author 이성수
 * @brief 플레이어 캐릭터의 임시 데이터를 저장하는 게임인스턴스 서브시스템입니다.
 * @since 2023-04-04
 */
UCLASS()
class GRADUATEPROJECT_API UGISS_TemporaryPlayerData : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGISS_TemporaryPlayerData();

	void InitializePlayerData();

	void SavePlayerData(int32 SlotIndex);

	void LoadPlayerData(int32 SlotIndex);

	/**
	* @brief 플레이어 캐릭터에게 강화 이펙트를 강화 수치만큼 적용합니다.
	* @param ReinforceEffect 적용할 강화 이펙트
	* @param ReinforceAmount 강화 이펙트의 강화 수치
	* @param bAddToArray 적용 후 해당 강화 정보를 ReinforceEffectsArray에 저장 여부
	* @see ReinforceEffectsArray 참고.
	*/
	void AssignReinforceEffect(const TSubclassOf<class UGameplayEffect>& ReinforceEffect, float ReinforceAmount, bool bAddToArray);



	UPROPERTY()
	float CurrentHealth;

	//UPROPERTY()
	//float CurrentMaxHealth;

	UPROPERTY()
	float CurrentSkillMana;

	UPROPERTY()
	class UItem_WeaponBase* EquippedWeapon;

	UPROPERTY()
	class UItem_ArmorBase* EquippedArmor;

	UPROPERTY()
	TMap<EEnemyAbilityID, class UItem_CharacterSkillBase*> EquippedSkills;

	/** @brief (강화 이펙트, 강화 이펙트 적용 수치)의 배열. 스테이지 이동 간에 강화 이펙트를 유지하기 위해 사용 */
	UPROPERTY()
	TArray<FReinforceEffectData> ReinforceEffectsArray;

	FString PlayerDataSaveName;
};
