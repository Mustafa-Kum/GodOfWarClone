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

	bShadersCompiled = false;

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

	PreloadShaders();
}

void UWarriorGameInstance::OnDestinationWorldLoaded(UWorld* LoadedWorld)
{
	GetMoviePlayer()->StopMovie();
	bShadersCompiled = false;
}

void UWarriorGameInstance::PreloadShaders()
{
	bShadersCompiled = false;

	if (GShaderCompilingManager)
	{
		GShaderCompilingManager->FinishAllCompilation();
	}

	WaitForShaderCompilation();
}

void UWarriorGameInstance::WaitForShaderCompilation()
{
	if (GShaderCompilingManager && GShaderCompilingManager->GetNumRemainingJobs() > 0)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(ShaderCompilationTimer, this, &ThisClass::WaitForShaderCompilation, 0.1f, false);
		}
		else
		{
			FTimerHandle TempTimer;
			GetTimerManager().SetTimer(TempTimer, this, &ThisClass::WaitForShaderCompilation, 0.1f, false);
		}
	}
	else
	{
		OnShaderCompilationComplete();
	}
}

void UWarriorGameInstance::OnShaderCompilationComplete()
{
	bShadersCompiled = true;
	GetMoviePlayer()->StopMovie();
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
