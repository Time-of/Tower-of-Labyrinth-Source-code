
#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "UObject/PrimaryAssetId.h"

#include "IconThumbnailInterface.h"

#include "ItemBase.generated.h"



/**
 * @author 이성수
 * @brief 아이템 베이스 클래스입니다.
 * @since 2023-02-21
 */
UCLASS(BlueprintType, Abstract)
class GRADUATEPROJECT_API UItemBase : public UPrimaryDataAsset, public IIconThumbnailInterface
{
	GENERATED_BODY()

public:
	UItemBase();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	/**
	* @brief 아이템 장착을 시도합니다.
	* @details 무기/방어구의 경우는 기존에 장착한 무기/방어구를 장착 해제하고 장착합니다.
	* @return 장착에 성공한 경우 true, 실패(캐릭터를 찾지 못한 경우)한 경우 false
	*/
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual bool TryEquipItem();

	UTexture2D* GetIcon_Implementation() const final;



	/** @brief **영문으로만 작성!!** 내부적으로 사용되는 아이템의 이름입니다. 주로 데이터 테이블 행 찾기에 사용됩니다. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
	FName InternalName;

	/** @brief 아이템의 이름 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
	FText ItemName;

	/** @brief 아이템의 설명 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
	FText ItemDescription;

	/** @brief 아이템의 타입 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item")
	FPrimaryAssetType ItemType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
	FSlateBrush ItemIcon;

	/** @brief 아이템의 가격 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
	float ItemPrice;

	/** @brief 아이템의 최대 수량 (0 이만인 경우 개수 제한 없음) */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
	int32 MaxCount;
};
