// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Dungeon_ArmoryGameMode.generated.h"

UCLASS(minimalapi)
class ADungeon_ArmoryGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADungeon_ArmoryGameMode();

	void BeginPlay();
};
