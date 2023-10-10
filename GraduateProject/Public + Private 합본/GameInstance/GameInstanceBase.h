
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Item/ItemBase.h"
#include "GameplayEffect.h"
#include "GameInstanceBase.generated.h"



USTRUCT(BlueprintType)
struct GRADUATEPROJECT_API FStatAttributeInfo
{
	GENERATED_BODY()

public:
	FStatAttributeInfo()
	{
		MaxHealth = 100.0f;
		Strength = 0.0f;
		Defense = 0.0f;
		AttackSpeed = 1.0f;
		MoveSpeed = 500.0f;
		Guard = 0.0f;
		GuardDamageTakenMult = 1.0f;
		DamageTakenMultOnGuardExists = 0.2f;
	}



	/** @brief 최대 체력 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxHealth;

	/** @brief 힘(공격력) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Strength;

	/** @brief 방어력 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Defense;

	/** @brief 공격속도 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AttackSpeed;

	/** @brief 이동속도 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MoveSpeed;

	/** @brief 가드 게이지 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Guard;

	/** @brief 가드 게이지에 받는 피해 배율 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float GuardDamageTakenMult;

	/** @brief 가드 게이지가 있을 때 체력에 받는 피해 배율 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DamageTakenMultOnGuardExists;
};



USTRUCT(BlueprintType)
struct GRADUATEPROJECT_API FLevelBasedStatAttributeTableRow : public FTableRowBase
{
	GENERATED_BODY()


public:
	/** @brief 레벨에 기반한 스탯 맵 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int32, FStatAttributeInfo> LevelBasedStatMap;



	/**
	* @brief 레벨에 비례하는 스탯 정보 가져오기
	* @details 만약 키 값이 존재하지 않는다면, 레벨로부터 추정치를 만들어 반환
	* @details **반드시 하나 이상의 키-값이 존재한다는 가정 하에 사용 가능!!!**
	*/
	FStatAttributeInfo GetStatByLevel(int32 CurrentLevel) const
	{
		// 만약 존재하는 값이라면 해당 스탯값을 반환
		if (LevelBasedStatMap.Contains(CurrentLevel))
			return LevelBasedStatMap[CurrentLevel];

		// 찾으려는 값보다 작은 최대 레벨
		int32 MaxLevelLessThanCurrentLevel = -1;

		// 맵에 들어있는 최대 레벨
		int32 MaxLevelInMap = -1;

		int32 MinLevelInMap = 1000;

		// 두 변수를 찾는다. (각각의 최대값)
		for (const auto& Item : LevelBasedStatMap)
		{
			if (MaxLevelInMap < Item.Key)
			{
				MaxLevelInMap = Item.Key;

				// 찾으려는 레벨보다 작은 경우만 저장
				MaxLevelLessThanCurrentLevel = 
					MaxLevelInMap < CurrentLevel ?
					MaxLevelInMap : MaxLevelLessThanCurrentLevel;
			}

			if (MinLevelInMap > Item.Key)
			{
				MinLevelInMap = Item.Key;
			}
		}

		// 한 개 뿐이라면 해당 스탯 정보를 반환
		if (MinLevelInMap == MaxLevelInMap)
			return LevelBasedStatMap[MinLevelInMap];

		// 해당 값이 초기화되지 않은 경우에 대한 예외 처리
		if (MaxLevelLessThanCurrentLevel == -1)
		{
			MaxLevelLessThanCurrentLevel = MinLevelInMap;
		}

		return InternalMakeStat(MaxLevelLessThanCurrentLevel, MaxLevelInMap, CurrentLevel);
	}



private:
	FStatAttributeInfo InternalMakeStat(int32 MaxLvLessThanTarget, int32 MaxLv, int32 TargetLevel) const
	{
		//float Alpha = TargetLevel / (MaxLv - MaxLvLessThanTarget);
		
		//float Alpha = static_cast<float>(UKismetMathLibrary::NormalizeToRange(TargetLevel, MaxLvLessThanTarget, TargetLevel));

		float Alpha = FMath::GetMappedRangeValueClamped(
			TRange<float>(static_cast<float>(MaxLvLessThanTarget), static_cast<float>(MaxLv)),
			TRange<float>(0.0f, 1.0f),
			static_cast<float>(TargetLevel)
			);

#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("현재렙보다 작은 최대렙: %d, 최대렙: %d, 타겟레벨: %d, 알파: %f"),
			MaxLvLessThanTarget, MaxLv, TargetLevel, Alpha);
#endif

		// 추정치 정보를 만들어 반환
		FStatAttributeInfo NewAttributeInfo;

		NewAttributeInfo.MaxHealth = FMath::Lerp<float>(LevelBasedStatMap[MaxLvLessThanTarget].MaxHealth,
			LevelBasedStatMap[MaxLv].MaxHealth,
			Alpha);

		NewAttributeInfo.Strength = FMath::Lerp<float>(LevelBasedStatMap[MaxLvLessThanTarget].Strength,
			LevelBasedStatMap[MaxLv].Strength,
			Alpha);

		NewAttributeInfo.Defense = FMath::Lerp<float>(LevelBasedStatMap[MaxLvLessThanTarget].Defense,
			LevelBasedStatMap[MaxLv].Defense,
			Alpha);

		NewAttributeInfo.AttackSpeed = FMath::Lerp<float>(LevelBasedStatMap[MaxLvLessThanTarget].AttackSpeed,
			LevelBasedStatMap[MaxLv].AttackSpeed,
			Alpha);

		NewAttributeInfo.MoveSpeed = FMath::Lerp<float>(LevelBasedStatMap[MaxLvLessThanTarget].MoveSpeed,
			LevelBasedStatMap[MaxLv].MoveSpeed,
			Alpha);

		NewAttributeInfo.Guard = FMath::Lerp<float>(LevelBasedStatMap[MaxLvLessThanTarget].Guard,
			LevelBasedStatMap[MaxLv].Guard,
			Alpha);

		NewAttributeInfo.GuardDamageTakenMult = FMath::Lerp<float>(LevelBasedStatMap[MaxLvLessThanTarget].GuardDamageTakenMult,
			LevelBasedStatMap[MaxLv].GuardDamageTakenMult,
			Alpha);

		NewAttributeInfo.DamageTakenMultOnGuardExists = FMath::Lerp<float>(LevelBasedStatMap[MaxLvLessThanTarget].DamageTakenMultOnGuardExists,
			LevelBasedStatMap[MaxLv].DamageTakenMultOnGuardExists,
			Alpha);

		return NewAttributeInfo;
	}
};



USTRUCT(BlueprintType)
struct GRADUATEPROJECT_API FWeightedItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** @brief 무작위 선택 가중치 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Weight = 0.0f;

	/** @brief 아이템 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UItemBase* Item;
};



USTRUCT(BlueprintType)
struct GRADUATEPROJECT_API FWeaponStatDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** @brief 무기 공격력 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AtkPower = 0.0f;

	/** @brief 무기 사거리 배율 (기본: 1.0) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AtkRangeRate = 1.0f;

	/** @brief 무기 공격속도 배율 (기본: 1.0) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AtkSpeedRate = 1.0f;
};



USTRUCT(BlueprintType)
struct GRADUATEPROJECT_API FArmorStatDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** @brief 가드 게이지 제공량 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float GuardGuageToAcquire = 0.0f;
};



/**
 * @author 이성수
 * @brief 게임에 사용될 게임인스턴스 베이스 클래스입니다.
 * @details BP 형태로 만들어 사용합니다.
 * @date 2023-02-23
 * @version 0.0.2
 */
UCLASS()
class GRADUATEPROJECT_API UGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:
	UGameInstanceBase();

	virtual void Init() override;

	struct FEnemyDropTableRow* GetEnemyDropTableRow(const FName& InternalEnemyName) const;

	/**
	* @brief FPrimaryAssetType 및 PrimaryAsset 이름으로 아이템(FPrimaryAssetId)을 가져옵니다.
	* @details PrimaryAssetName은 아이템 데이터 애셋의 이름입니다. (예: Item_Weapon_TestWeapon)
	*/
	FPrimaryAssetId GetItemByTypeAndName(const FPrimaryAssetType& PrimaryAssetType, const FName& PrimaryAssetName);

	TSubclassOf<class AItemPickupBase> GetItemPickupClass() const;

	/**
	* @brief GISS_InteractableNpcData에서 기본값으로 사용될 '제작 가능한 아이템' 데이터입니다.
	* @details 세이브 데이터가 있다면 세이브된 데이터의 것을 사용하고, 아니라면 해당 값을 사용합니다.
	* @see UGISS_InteractableNPCData 참고.
	*/
	UPROPERTY(EditAnywhere, Category = "InteractableNpcData")
	TMap<FPrimaryAssetId, bool> Default_Shop_CraftItem_CanBeCrafted_Map;

	UItemBase* GetRandomItemFromWeightedWeaponTable() const;

	UItemBase* GetWeaponFromWeightedWeaponTableInWeightRange(float MinWeight, float MaxWeight);

	UFUNCTION(BlueprintCallable, Category = "GameInstance|Item")
	UItemBase* GetRandomItemFromWeightedTable(UDataTable* WeightedTable) const;

	UItemBase* _GetRandomItemInWeightedRows(const TArray<FWeightedItemTableRow*>& RowArray) const;

	float GetWeaponWeightInWeightedTable(const FName& ItemInternalName);

	FStatAttributeInfo GetStatByLevel(const FName& InternalName, int32 Level) const;

	FGameplayEffectSpecHandle MakeWeaponStatEffect(const FName& InternalName, class UAbilitySystemComponent* ASC);

	FWeaponStatDataTableRow* GetWeaponStat(const FName& InternalName) const;

	FGameplayEffectSpecHandle MakeArmorStatEffect(const FName& InternalName, class UAbilitySystemComponent* ASC);

	FArmorStatDataTableRow* GetArmorStat(const FName& InternalName) const;

	UFUNCTION(BlueprintCallable)
	void ShowLoadingScreen();

	UFUNCTION(BlueprintCallable)
	void HideLoadingScreen();



protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "GameInstance|Item")
	void InitializeItems(const TArray<FPrimaryAssetId>& PrimaryAssetIdList);



	TArray<FPrimaryAssetType> ItemTypeArray;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "GameInstance|Item")
	TArray<class UItemBase*> ItemsArray;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GameInstance|Table|DropTable")
	class UDataTable* EnemyDropTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GameInstance|Table")
	class UDataTable* WeightedWeaponTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GameInstance|Table")
	class UDataTable* EnemyStatAttributeTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GameInstance|Table")
	class UDataTable* WeaponStatDataTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GameInstance|Table")
	class UDataTable* ArmorStatDataTable;

	UPROPERTY()
	TSubclassOf<class AItemPickupBase> ItemPickupClass;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> WeaponStatEffectBaseClass;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> ArmorStatEffectBaseClass;

	UPROPERTY()
	TSubclassOf<UUserWidget> LoadingScreenWidget;

	UPROPERTY()
	UUserWidget* LoadingScreenInstance;
};
