// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/Dungeon_ArmoryGameMode.h"
#include "Characters/Dungeon_ArmoryCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

ADungeon_ArmoryGameMode::ADungeon_ArmoryGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Player/BP_PlayerableCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{

		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ADungeon_ArmoryGameMode::BeginPlay()
{
	Super::BeginPlay();

}
