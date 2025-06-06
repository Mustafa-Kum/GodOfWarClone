// KumDev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameMods/WarriorBaseGameMod.h"
#include "WarriorSurvivalGameMod.generated.h"

UENUM(BlueprintType)
enum class EWarriorSurvivalGameModState : uint8
{
	WaitSpawnNewWave,
	SpawningNewWave,
	InProgress,
	WaveCompleted,
	AllWavesDone,
	PlayerDied
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvivalGameModStateChangedDelegate, EWarriorSurvivalGameModState, CurrentState);

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorSurvivalGameMod : public AWarriorBaseGameMod
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void SetCurrentSurvivalGameModState(EWarriorSurvivalGameModState InState);

	UPROPERTY()
	EWarriorSurvivalGameModState CurrentSurvivalGameModState;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSurvivalGameModStateChangedDelegate OnSurvivalGameModStateChanged;
};
