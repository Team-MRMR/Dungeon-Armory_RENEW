// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "IMovableTask.generated.h"

class UBehaviorTreeComponent;

UINTERFACE()
class DUNGEON_ARMORY_API UMovableTask : public UInterface
{
    GENERATED_BODY()
};

class DUNGEON_ARMORY_API IMovableTask
{
    GENERATED_BODY()

public:
	virtual void OnMoveCompleted(UBehaviorTreeComponent* OwnerComp) = 0;
};
