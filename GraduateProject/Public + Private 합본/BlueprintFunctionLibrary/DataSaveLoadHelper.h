
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataSaveLoadHelper.generated.h"



/**
 * @author 이성수
 * @brief 데이터를 저장하고 로드하는 행동을 쉽게 사용할 수 있도록 만들어진 헬퍼 클래스입니다.
 * @since 2023-04-04
 */
UCLASS()
class GRADUATEPROJECT_API UDataSaveLoadHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "DataSaveLoadHelper")
	static void InitializeGameData(AActor* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "DataSaveLoadHelper")
	static void SaveGameData(AActor* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "DataSaveLoadHelper")
	static void LoadGameData(AActor* WorldContext);

	/**
	* @brief 플레이어 데이터를 보존한 채로 레벨을 이동합니다.
	* @param WorldContext 현재 월드를 알기 위한 컨텍스트 액터
	* @param LevelName 이동할 레벨 이름
	* @param bDoNotStreaming true라면 심리스 트래블하지 않고 레벨 이름 단위로 가져옵니다. false라면 /Game/Level-Designs/ 폴더에 있는 레벨을 트랜지션으로 이동합니다.
	*/
	UFUNCTION(BlueprintCallable, Category = "DataSaveLoadHelper")
	static void OpenLevelWithPlayerData(AActor* WorldContext, const FName& LevelName, bool bDoNotSeamless = false);

	/**
	* @brief 마을로 진입합니다.
	* @details 마을 진입 전에, 임시 데이터를 초기화하고 무기, 방어구, 스킬을 모두 리로드합니다.
	* @see UGISS_PlayerInventory::ReloadAllWeaponsArmorsSkills() 참고.
	*/
	UFUNCTION(BlueprintCallable, Category = "DataSaveLoadHelper")
	static void OpenTownLevel(AActor* WorldContext, const FName& LevelName);

	UFUNCTION(BlueprintCallable, Category = "DataSaveLoadHelper")
	static void OpenTitleLevel(AActor* WorldContext);

	static const int32 SlotIndex = 1;
};
