
#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GameAssetManager.generated.h"



/**
 * @author 이성수
 * @brief Gameplay Ability System을 사용하기 위한 설정을 진행하는 애셋 매니저 클래스입니다.
 * @since 2023-01-31
 */
UCLASS()
class GRADUATEPROJECT_API UGameAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	UGameAssetManager();

	static UGameAssetManager& GetInstance();

	class UItemBase* LoadItem(const FPrimaryAssetId& PrimaryAssetId);

	virtual void StartInitialLoading() override;

	static const FPrimaryAssetType ItemType_Weapon;
	
	static const FPrimaryAssetType ItemType_Armor;

	static const FPrimaryAssetType ItemType_Potion;

	static const FPrimaryAssetType ItemType_Material;

	static const FPrimaryAssetType ItemType_CharacterSkill;
};
