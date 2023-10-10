
#include "Widget/NPCTrainingWidget.h"

#include "CharacterBase.h"
#include "Enemy/Abilities/AbilitySystemCompBase.h"
#include "GameInstance/GISS_PlayerInventory.h"
#include "GameInstance/GISS_PlayerMoney.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"



UNPCTrainingWidget::UNPCTrainingWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	SelectedAttributeType(ETrainingAttributeType::NONE),
	ExecuteReinforceButton(nullptr), MoneyText(nullptr),
	ReinforceDescriptionText(nullptr)
{
	
}



void UNPCTrainingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UGISS_PlayerMoney* MoneySS = GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>())
	{
		UpdateMoneyText(MoneySS->GetMoney());

		MoneySS->OnMoneyChanged.AddUniqueDynamic(this, &UNPCTrainingWidget::UpdateMoneyText);
	}
}



void UNPCTrainingWidget::UpdateMoneyText(float NewAmount)
{
#define LOCTEXT_NAMESPACE "TrainingMoneyTextNameSpace"
	MoneyText->SetText(FText::Format(LOCTEXT("MoneyText", "GOLD: {0}"), FText::AsNumber(NewAmount)));
#undef LOCTEXT_NAMESPACE
}



void UNPCTrainingWidget::SelectTrainingAttributeType(ETrainingAttributeType TypeToSelect)
{
	if (SelectedAttributeType == TypeToSelect) return;

	SelectedAttributeType = TypeToSelect;

	if (IsValid(ExecuteReinforceButton))
	{
		ExecuteReinforceButton->SetIsEnabled(SelectedAttributeType != ETrainingAttributeType::NONE);
	}
}



bool UNPCTrainingWidget::ExecuteTrainingReinforce()
{
	TSubclassOf<UGameplayEffect> EffectToApply;
	FGameplayTagContainer TagContainer;
	UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();

	// 타입에 따라 필요 변수 설정
	switch (SelectedAttributeType)
	{
		default:
		case ETrainingAttributeType::NONE:
			return false;

		case ETrainingAttributeType::MAXHEALTH:
			EffectToApply = InvSS->Effect_MaxHealthReinforce;
			TagContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Effect.Reinforce.Training.MaxHealth")));
			break;

		case ETrainingAttributeType::DEFENSE:
			EffectToApply = InvSS->Effect_DefenseReinforce;
			TagContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Effect.Reinforce.Training.Defense")));
			break;

		case ETrainingAttributeType::STRENGTH:
			EffectToApply = InvSS->Effect_StrengthReinforce;
			TagContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Effect.Reinforce.Training.Strength")));
			break;
	}

	// 커브 테이블에서 가격 정보 읽어오기
	float SelectedAttributePrice = GetSelectedAttributeTrainingPrice();

	UGISS_PlayerMoney* PlayerMoneySS = GetGameInstance()->GetSubsystem<UGISS_PlayerMoney>();

	// 유효하지 않거나 돈 사용 실패 시 false 반환
	if (PlayerMoneySS == nullptr || !PlayerMoneySS->TryUseMoney(SelectedAttributePrice)) return false;

	// 레벨 증가
	InvSS->TrainingAttributeLevelMap[SelectedAttributeType]++;

	ACharacterBase* Character = GetWorld()->GetFirstPlayerController()->GetPawn<ACharacterBase>();

	if (!IsValid(Character)) return false;

	// 이펙트 적용
	if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
	{
		ASC->RemoveActiveEffectsWithTags(TagContainer);

		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectToApply,
			InvSS->TrainingAttributeLevelMap[SelectedAttributeType], ASC->MakeEffectContext());

		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		return true;
	}

	return false;
}



float UNPCTrainingWidget::GetSelectedAttributeTrainingPrice() const
{
	float OutPrice;
	FName CurveRowName;

	// 타입에 따라 필요 변수 설정
	switch (SelectedAttributeType)
	{
		default:
		case ETrainingAttributeType::NONE:
			return 0.0f;

		case ETrainingAttributeType::MAXHEALTH:
			CurveRowName = TEXT("MaxHealthReinforcePrice");
			break;

		case ETrainingAttributeType::DEFENSE:
			CurveRowName = TEXT("DefenseReinforcePrice");
			break;

		case ETrainingAttributeType::STRENGTH:
			CurveRowName = TEXT("StrengthReinforcePrice");
			break;
	}

	TEnumAsByte<EEvaluateCurveTableResult::Type> OutCurveEvaluateResult;

	UGISS_PlayerInventory* InvSS = GetGameInstance()->GetSubsystem<UGISS_PlayerInventory>();

	// 커브 테이블에서 가격 정보 읽어오기
	UDataTableFunctionLibrary::EvaluateCurveTableRow(TrainingReinforceCurveTable,
		CurveRowName, InvSS->TrainingAttributeLevelMap[SelectedAttributeType],
		OutCurveEvaluateResult, OutPrice, FString(""));

	return FMath::RoundToInt(OutPrice);
}



void UNPCTrainingWidget::SetReinforceDescriptionText()
{
//#define LOCTEXT_NAMESPACE "TrainingReinforceDescriptionNameSpace"
	FString ReinforceTextFormat = TEXT("강화할 능력치: {0}\n비용: {1}");
	FString ReinforceName;
	switch (SelectedAttributeType)
	{
	default:
	case ETrainingAttributeType::NONE:
		ReinforceName = TEXT("선택되지 않음");
		break;

	case ETrainingAttributeType::MAXHEALTH:
		ReinforceName = TEXT("최대 체력");
		break;

	case ETrainingAttributeType::DEFENSE:
		ReinforceName = TEXT("방어력");
		break;

	case ETrainingAttributeType::STRENGTH:
		ReinforceName = TEXT("공격력");
		break;
	}

	ReinforceDescriptionText->SetText(FText::Format(FText::FromString(ReinforceTextFormat), FText::FromString(ReinforceName), FText::AsNumber(GetSelectedAttributeTrainingPrice())));
//#undef LOCTEXT_NAMESPACE
}
