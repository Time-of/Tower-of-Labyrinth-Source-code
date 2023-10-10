
#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "Item/ItemSlot.h"

#include "GISS_PlayerInventory.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInventoryChanged, UItemBase*, Item, int32, NewQuantity, int32, DeltaQuantity);



UENUM(BlueprintType)
enum class ETrainingAttributeType : uint8
{
	NONE, MAXHEALTH, DEFENSE, STRENGTH
};



/**
 * @author 이성수
 * @brief 플레이어의 인벤토리를 담당하는 게임인스턴스 서브시스템 클래스입니다.
 * @since 2023-03-03
 */
UCLASS()
class GRADUATEPROJECT_API UGISS_PlayerInventory : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGISS_PlayerInventory();

	/**
	* @brief 아이템을 Quantity만큼 개수를 추가합니다. (음수인 경우 뺌) 가급적이면 이 함수를 사용하세요. (관련 콜백 기능 때문)
	* @details 아이템을 가진 적이 없다면 새로 Quantity 만큼 추가하고, 이미 있었다면 Quantity 만큼 더합니다.
	* @details OnInventoryChangedDelegate를 발생시킵니다.
	*/
	void AddItem(const FPrimaryAssetId& PrimaryAssetId, int32 Quantity);

	/**
	* @brief 아이템을 Quantity만큼 개수를 추가합니다. (음수인 경우 뺌) 가급적이면 이 함수를 사용하세요. (관련 콜백 기능 때문)
	* @details 아이템을 가진 적이 없다면 새로 Quantity 만큼 추가하고, 이미 있었다면 Quantity 만큼 더합니다.
	* @details OnInventoryChangedDelegate를 발생시킵니다.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(UItemBase* Item, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetItemQuantity(UItemBase* Item, int32 Quantity);

	/**
	* @brief 아이템의 수량을 가져옵니다.
	* @return 가져왔다면 수량을 return, 가져오지 못했다면 -1
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 TryGetItemQuantity(UItemBase* Item);

	UFUNCTION(BlueprintCallable)
	void InitializeInventoryData();

	UFUNCTION(BlueprintCallable)
	void SaveInventoryData(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void LoadInventoryData(int32 SlotIndex);

	/**
	* @brief 현재 인벤토리에서 무기, 방어구, 스킬을 전부 지우고, 세이브게임에서 무기, 방어구만 전부 로드합니다.
	* @see UDataSaveLoadHelper::OpenTownLevel() 참고.
	*/
	UFUNCTION(BlueprintCallable)
	void ReloadAllWeaponsArmorsSkills(int32 SlotIndex);

	/**
	* @brief 현재 TrainingAttributeLevelMap을 기준으로 모든 훈련 이펙트 적용하기
	* @details 인벤토리 SaveGame을 로드할 때에도 사용되고,
	* @details PlayerCharacterBase가 임시 데이터(레벨이 넘어가도 저장되는 데이터)를 로드할 때 사용됩니다.
	*/
	void ApplyAllTrainingEffects();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChangedDelegate;

	

	FString InventorySaveName;

	UPROPERTY()
	TMap<UItemBase*, FItemSlot> InventoryMap;

	UPROPERTY()
	TMap<ETrainingAttributeType, float> TrainingAttributeLevelMap;

	UPROPERTY()
	TSubclassOf<class UGameplayEffect> Effect_MaxHealthReinforce;

	UPROPERTY()
	TSubclassOf<class UGameplayEffect> Effect_DefenseReinforce;

	UPROPERTY()
	TSubclassOf<class UGameplayEffect> Effect_StrengthReinforce;
};
