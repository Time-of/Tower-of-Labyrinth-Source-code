
#include "LoadingGameMode.h"

#include "GameInstance/GameInstanceBase.h"



void ALoadingGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);

	UGameInstanceBase* GIBase = GetGameInstance<UGameInstanceBase>();

	GIBase->ShowLoadingScreen();
}



ALoadingGameMode::ALoadingGameMode()
{
	bUseSeamlessTravel = true;
}



void ALoadingGameMode::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	UGameInstanceBase* GIBase = GetGameInstance<UGameInstanceBase>();

	GIBase->HideLoadingScreen();
}



void ALoadingGameMode::StartToLeaveMap()
{
	Super::StartToLeaveMap();

	UGameInstanceBase* GIBase = GetGameInstance<UGameInstanceBase>();

	GIBase->ShowLoadingScreen();
}
