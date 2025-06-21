// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Characters/Core/AI/Interface/IMovableTask.h"

#include "BTTask_MoveToPoint.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class DUNGEON_ARMORY_API UBTTask_MoveToPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_MoveToPoint();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
