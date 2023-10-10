
#pragma once

#include "CoreMinimal.h"



/**
 * @author 이성수
 * @brief 블랙보드에 사용되는 블랙보드 키 값들을 사용하기 편리하도록 모아 놓은 클래스입니다.
 * @since 2023-02-01
 */
namespace BBKeys
{
	static const FName TargetActor(TEXT("TargetActor"));

	static const FName bHasLineOfSight(TEXT("bHasLineOfSight"));

	static const FName InitialLocation(TEXT("InitialLocation"));

	static const FName PatrolLocation(TEXT("PatrolLocation"));

	static const FName DistanceFromTarget(TEXT("DistanceFromTarget"));

	static const FName TargetLocation(TEXT("TargetLocation"));

	static const FName bHaveToDoPatrol(TEXT("bHaveToDoPatrol"));
}
