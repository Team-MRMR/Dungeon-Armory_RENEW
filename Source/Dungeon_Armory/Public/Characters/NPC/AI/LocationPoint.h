// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LocationPoint.generated.h"

UCLASS(Blueprintable, BlueprintType)
class DUNGEON_ARMORY_API ALocationPoint : public AActor
{
	GENERATED_BODY()

/***** Unreal *****/
public:
	ALocationPoint();

protected:
	virtual void BeginPlay() override;

/***** Behavior Tree *****/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Point")
	TArray<float> WaitTimes;	// 대기 시간 (초 단위)
};
