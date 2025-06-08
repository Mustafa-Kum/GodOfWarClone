// KumDev All Rights Reserved


#include "GameMods/WarriorSurvivalGameMod.h"
#include "Engine/AssetManager.h"
#include "Characters/WarriorEnemyCharacter.h"

#include "WarriorDebugHelper.h"

void AWarriorSurvivalGameMod::BeginPlay()
{
	Super::BeginPlay();

	checkf(EnemyWaveSpawnerDataTable, TEXT("EnemyWaveSpawnerDataTable is not set in AWarriorSurvivalGameMod! Please set it in the editor."));

	SetCurrentSurvivalGameModState(EWarriorSurvivalGameModState::WaitSpawnNewWave);

	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();

	PreLoadNextWaveEnemies();
}

void AWarriorSurvivalGameMod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentSurvivalGameModState == EWarriorSurvivalGameModState::WaitSpawnNewWave)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
		{
			TimePassedSinceStart = 0.f;

			SetCurrentSurvivalGameModState(EWarriorSurvivalGameModState::SpawningNewWave);
		}
	}

	if (CurrentSurvivalGameModState == EWarriorSurvivalGameModState::SpawningNewWave)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= SpawnEnemiesDelayTime)
		{
			TimePassedSinceStart = 0.f;

			SetCurrentSurvivalGameModState(EWarriorSurvivalGameModState::InProgress
			
			);
		}
	}

	if (CurrentSurvivalGameModState == EWarriorSurvivalGameModState::WaveCompleted)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= WaveCompletedWaitTime)
		{
			TimePassedSinceStart = 0.f;

			CurrentWaveCount++;	

			if (HasFinishedAllWaves())
			{
				SetCurrentSurvivalGameModState(EWarriorSurvivalGameModState::AllWavesDone);
			}
			else
			{
				SetCurrentSurvivalGameModState(EWarriorSurvivalGameModState::WaitSpawnNewWave);

				PreLoadNextWaveEnemies();
			}
		}
	}
}

void AWarriorSurvivalGameMod::SetCurrentSurvivalGameModState(EWarriorSurvivalGameModState InState)
{
	CurrentSurvivalGameModState = InState;
	
	OnSurvivalGameModStateChanged.Broadcast(CurrentSurvivalGameModState);
}

bool AWarriorSurvivalGameMod::HasFinishedAllWaves() const
{
	return CurrentWaveCount > TotalWavesToSpawn;
}

void AWarriorSurvivalGameMod::PreLoadNextWaveEnemies()
{
	if (HasFinishedAllWaves())
	{
		return;
	}

	for (const FWarriorEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull())
		{
			continue;
		}

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[SpawnerInfo, this]()
				{
					if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
					{
						PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);

						Debug::Print(LoadedEnemyClass->GetName() + TEXT(" is Loaded"));
					}
				}
			)
		);
	}
}

FWarriorEnemyWaveSpawnerTableRow* AWarriorSurvivalGameMod::GetCurrentWaveSpawnerTableRow() const
{
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));

	FWarriorEnemyWaveSpawnerTableRow* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FWarriorEnemyWaveSpawnerTableRow>(RowName, FString());

	checkf(FoundRow, TEXT("Could Not Find a Valid Row Under the Name %s in the Data Table"), *RowName.ToString());

	return FoundRow;
}
