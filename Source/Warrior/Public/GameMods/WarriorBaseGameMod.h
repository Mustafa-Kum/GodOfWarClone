// KumDev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "WarriorBaseGameMod.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorBaseGameMod : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	FORCEINLINE EWarriorGameDifficulty GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

	AWarriorBaseGameMod();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	EWarriorGameDifficulty CurrentGameDifficulty;
};
