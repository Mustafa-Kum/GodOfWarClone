// KumDev All Rights Reserved


#include "GameMods/WarriorSurvivalGameMod.h"

void AWarriorSurvivalGameMod::BeginPlay()
{
	Super::BeginPlay();

	checkf(EnemyWaveSpawnerDataTable, TEXT("EnemyWaveSpawnerDataTable is not set in AWarriorSurvivalGameMod! Please set it in the editor."));

	SetCurrentSurvivalGameModState(EWarriorSurvivalGameModState::WaitSpawnNewWave);

	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();
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
