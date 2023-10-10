// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


DECLARE_LOG_CATEGORY_EXTERN(LoadPlayerDataLog, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(ShuffleLevelLog, Log, All);



/**
 * @author 이성수
 * @brief 게임플레이 어빌리티 ID로, 주로 인풋 매핑을 위해 사용
 */
UENUM(BlueprintType)
enum class EEnemyAbilityID : uint8
{
	None UMETA(DisplayName = "None"),
	Confirm UMETA(DisplayName = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel"),
	AbilityQ UMETA(DisplayName = "Ability_Q"),
	AbilityE UMETA(DisplayName = "Ability_E"),
	AbilityF UMETA(DisplayName = "Ability_F"),
	AbilityRM UMETA(DisplayName = "Ability_RightMouse")
};



/**
 * @author 이성수
 * @brief 상점 NPC 위젯에 사용되는 상점 상호작용 타입 열거형
 * @date 2023-02-26
 * @version 0.0.1
 */
UENUM(BlueprintType)
enum class EShopInteractType : uint8
{
	NONE, BUY, SELL, CRAFT
};