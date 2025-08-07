// KumDev All Rights Reserved


#include "WarriorGameInstance.h"
#include "MoviePlayer.h"
#include "Engine/Engine.h"
#include "RenderingThread.h"
#include "ShaderCompiler.h"
#include "TimerManager.h"

void UWarriorGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnDestinationWorldLoaded);
}

void UWarriorGameInstance::OnPreLoadMap(const FString& MapName)
{
	FLoadingScreenAttributes LoadingScreenAttributes;

	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = false;
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.0f;
	LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);

}

void UWarriorGameInstance::OnDestinationWorldLoaded(UWorld* LoadedWorld)
{
	//GetMoviePlayer()->StopMovie();
}

TSoftObjectPtr<UWorld> UWarriorGameInstance::GetGameLevelByTag(FGameplayTag InTag) const
{
	for (const FWarriorGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if (!GameLevelSet.IsValid())
		{
			continue;
		}

		if (GameLevelSet.LevelTag == InTag)
		{
			return GameLevelSet.Level;
		}
	}

	return TSoftObjectPtr<UWorld>();
}

void UWarriorGameInstance::StopLoadingScreen()
{
	GetMoviePlayer()->StopMovie();
}
