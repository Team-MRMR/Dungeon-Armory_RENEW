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
	FORCEINLINE FVector GetLocation() const { return FVector(Location.X, Location.Y, 0.0f); }
	FORCEINLINE float GetWaitTime() const { return WaitTime; }

protected:
	FVector Location;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Point")
	float WaitTime;		// 대기 시간 (초 단위)
};
