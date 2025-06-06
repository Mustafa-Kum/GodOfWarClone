// KumDev All Rights Reserved


#include "GameMods/WarriorSurvivalGameMod.h"

void AWarriorSurvivalGameMod::BeginPlay()
{
	Super::BeginPlay();
}

void AWarriorSurvivalGameMod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWarriorSurvivalGameMod::SetCurrentSurvivalGameModState(EWarriorSurvivalGameModState InState)
{
	CurrentSurvivalGameModState = InState;
	
	OnSurvivalGameModStateChanged.Broadcast(CurrentSurvivalGameModState);
}
