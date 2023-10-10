
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GISS_ShuffleLevel.generated.h"



USTRUCT(BlueprintType)
struct GRADUATEPROJECT_API FLevelNameInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** @brief 해당 테마의 레벨 이름 배열 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> LevelNameArray;

	/** @brief 해당 테마의 보스 레벨 이름 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName BossLevelName;

	/** @brief 해당 테마의 인카운터(이벤트) 레벨 이름 배열 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> EncounterLevelNameArray;
};



/**
 * @author 이성수
 * @brief 레벨 순서를 셔플하는 게임인스턴스 서브시스템입니다.
 * @since 2023-05-10
 */
UCLASS()
class GRADUATEPROJECT_API UGISS_ShuffleLevel : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGISS_ShuffleLevel();

	UFUNCTION(BlueprintCallable)
	void MakeShuffledLevelNameArray();

	FName GetLevelNameByIndex(int32 Index) const;



	UPROPERTY(BlueprintReadWrite)
	int32 CurrentLevelCount;



protected:
	void MakeLevels(TArray<FLevelNameInfo*>& TableRowArray, bool bAddEncounterToEnd = true);

	void ShuffleStringArray(TArray<FName>& StringArray);

	void ShuffleTableRowArray(TArray<FLevelNameInfo*>& TableRowArray);

	void AddEncounterLevelInArray(TArray<FName>& ArrayToInsert, int32& EncounterLevelIndex, const TArray<FName>& EncountersArray);

	/** @brief 디버그용으로, 모든 셔플된 배열을 출력합니다. */
	void PrintAllShuffledLevelNameArrayForDebug() const;



	/** @brief 엔드 스테이지 전에 배치될 일반 레벨 이름 정보들 */
	UPROPERTY()
	UDataTable* BasicLevelNameInfoTable;

	/** @brief 엔드 스테이지 레벨 이름 정보들 */
	UPROPERTY()
	UDataTable* EndLevelNameInfoTable;

	UPROPERTY(BlueprintReadOnly)
	TArray<FName> ShuffledLevelNameArray;
};
