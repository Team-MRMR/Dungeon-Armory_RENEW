// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TeamInfo.generated.h"

UENUM(BlueprintType)
enum class ETeamType : uint8
{
    None = 0 UMETA(DisplayName = "None"),

    Player   UMETA(DisplayName = "Player"),
    NPC      UMETA(DisplayName = "NPC"),
    Mob      UMETA(DisplayName = "Mob"),

	Size
};

UENUM(BlueprintType)
enum class ERelationType : uint8
{
    None = 0    UMETA(DisplayName = "None"),

    Neutral     UMETA(DisplayName = "Neutral"),
    Friendly    UMETA(DisplayName = "Friendly"),
    Hostile     UMETA(DisplayName = "Hostile"),

    Size
};

USTRUCT(BlueprintType)
struct FTeamRelation
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
    TMap<ETeamType, ERelationType> RelationMap;

};