// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "IMovableTask.generated.h"

class UBehaviorTreeComponent;
	
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIMovableTask : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUNGEON_ARMORY_API IIMovableTask
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnMoveCompleted(UBehaviorTreeComponent* OwnerComp) = 0;
};
