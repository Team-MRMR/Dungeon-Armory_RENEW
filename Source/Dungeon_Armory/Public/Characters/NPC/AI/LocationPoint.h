// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "LocationPoint.generated.h"

UCLASS()
class DUNGEON_ARMORY_API ALocationPoint : public AActor
{
	GENERATED_BODY()

/***** Functions (Unreal) *****/
public:	
	ALocationPoint();

/***** Patrol *****/
public:
	UFUNCTION(BlueprintCallable, Category = "LocationPoint")
	FORCEINLINE FVector GetPointLocation() const { return GetActorLocation(); }

};
