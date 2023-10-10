
#include "GameInstance/GISS_ShuffleLevel.h"

#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include <GraduateProject/GraduateProject.h>



UGISS_ShuffleLevel::UGISS_ShuffleLevel() : Super(),
	CurrentLevelCount(0),
	BasicLevelNameInfoTable(nullptr),
	EndLevelNameInfoTable(nullptr),
	ShuffledLevelNameArray()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> BasicInfoAsset(TEXT("/Game/SungSoo_Workspace/DataTables/LevelShuffle/BasicLevelNameInfoTable"));
	if (BasicInfoAsset.Succeeded())
	{
		BasicLevelNameInfoTable = BasicInfoAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> EndInfoAsset(TEXT("/Game/SungSoo_Workspace/DataTables/LevelShuffle/EndLevelNameInfoTable"));
	if (EndInfoAsset.Succeeded())
	{
		EndLevelNameInfoTable = EndInfoAsset.Object;
	}
}



void UGISS_ShuffleLevel::MakeShuffledLevelNameArray()
{
	ShuffledLevelNameArray.Reset();
	CurrentLevelCount = 0;

	TArray<FLevelNameInfo*> OutNormalLevelRows;
	BasicLevelNameInfoTable->GetAllRows<FLevelNameInfo>(TEXT(""), OutNormalLevelRows);

	TArray<FLevelNameInfo*> OutEndLevelRows;
	EndLevelNameInfoTable->GetAllRows<FLevelNameInfo>(TEXT(""), OutEndLevelRows);


	// 노말 레벨, 엔드 레벨을 각각 셔플하기
	ShuffleTableRowArray(OutNormalLevelRows);
	ShuffleTableRowArray(OutEndLevelRows);


	// 노말 레벨, 엔드 레벨을 최종 레벨 배열에 붙여서 완성하기
	MakeLevels(OutNormalLevelRows);
	MakeLevels(OutEndLevelRows, false);

	PrintAllShuffledLevelNameArrayForDebug();
}



FName UGISS_ShuffleLevel::GetLevelNameByIndex(int32 Index) const
{
	if (Index >= ShuffledLevelNameArray.Num()) return FName("LEVEL_END");

	return ShuffledLevelNameArray[Index];
}



void UGISS_ShuffleLevel::MakeLevels(TArray<FLevelNameInfo*>& TableRowArray, bool bAddEncounterToEnd)
{
	// 각각의 노말 테마마다
	for (const auto& Row : TableRowArray)
	{
		// 해당 테마의 결과 배열
		TArray<FName> NormalLevelTheme;

		// 현 테마의 인카운터 목록 가져오기
		TArray<FName>& EncountersArray = Row->EncounterLevelNameArray;

		// 인카운터 레벨 이름 섞기
		ShuffleStringArray(EncountersArray);
		int32 EncounterLevelIndex = 0;

		// 각 레벨 이름 섞기
		ShuffleStringArray(Row->LevelNameArray);

		// 무조건 첫 스테이지는 전투 스테이지로 배치
		NormalLevelTheme.Add(Row->LevelNameArray[0]);


		TArray<FName> TempArray;

		// 2~4 스테이지를 배치하는 과정
		// 전투 스테이지 2개, 인카운터 스테이지 1개를 임시 배열에 넣고 셔플
		int32 Length = Row->LevelNameArray.Num();
		for (int32 i = 1, Count = 0; i < Length && Count < 2; ++i, ++Count)
		{
			TempArray.Add(Row->LevelNameArray[i]);
		}
		AddEncounterLevelInArray(TempArray, EncounterLevelIndex, EncountersArray);

		ShuffleStringArray(TempArray);

		// 셔플한 임시 배열을 노말 레벨 테마에 추가
		// 이로써 2~4 스테이지는 이벤트 레벨 1개 + 전투 레벨 2개가 뒤섞여 들어간 효과.
		NormalLevelTheme.Append(TempArray);

		// 노말 레벨 테마에 보스 레벨 붙이기
		NormalLevelTheme.Add(Row->BossLevelName);

		// 다음 이벤트 레벨 붙이기
		if (bAddEncounterToEnd)
			AddEncounterLevelInArray(NormalLevelTheme, EncounterLevelIndex, EncountersArray);


		// 최종 배열에 현재 테마 결과를 붙이기
		ShuffledLevelNameArray.Append(NormalLevelTheme);
	}
}



void UGISS_ShuffleLevel::ShuffleStringArray(TArray<FName>& StringArray)
{
	int32 Length = StringArray.Num();

	for (int32 i = Length - 1; i > 0; --i)
	{
		int32 j = UKismetMathLibrary::RandomIntegerInRange(0, i);
		StringArray.Swap(i, j);
	}
}



void UGISS_ShuffleLevel::ShuffleTableRowArray(TArray<FLevelNameInfo*>& TableRowArray)
{
	int32 Length = TableRowArray.Num();

	for (int32 i = Length - 1; i > 0; --i)
	{
		int32 j = UKismetMathLibrary::RandomIntegerInRange(0, i);
		TableRowArray.Swap(i, j);
	}
}



void UGISS_ShuffleLevel::AddEncounterLevelInArray(TArray<FName>& ArrayToInsert, int32& EncounterLevelIndex, const TArray<FName>& EncountersArray)
{
	if (EncounterLevelIndex >= EncountersArray.Num())
	{
	#if WITH_EDITOR
		UE_LOG(ShuffleLevelLog, Warning, 
			TEXT("레벨 셔플 시스템: 현재 %s이 들어있는 배열에 인카운터 레벨을 넣으려 하는데, 인카운터 레벨 수가 모자랍니다! 따라서 추가하지 못했습니다. 인덱스: %d"),
			*ArrayToInsert[0].ToString(), EncounterLevelIndex);
	#endif

		return;
	}

	ArrayToInsert.Add(EncountersArray[EncounterLevelIndex++]);
}



void UGISS_ShuffleLevel::PrintAllShuffledLevelNameArrayForDebug() const
{
#if WITH_EDITOR
	int32 Length = ShuffledLevelNameArray.Num();

	UE_LOG(ShuffleLevelLog, Warning, TEXT("레벨 셔플 시스템: 셔플 결과 출력 중... 총 개수는 %d개입니다."),
		Length);

	for (int32 i = 0; i < Length; ++i)
	{
		UE_LOG(ShuffleLevelLog, Warning, TEXT("레벨 셔플 시스템: %d 인덱스의 레벨 %s"),
			i, *ShuffledLevelNameArray[i].ToString());
	}

	UE_LOG(ShuffleLevelLog, Warning, TEXT("레벨 셔플 시스템: 셔플 결과 출력 완료."));
#endif
}
