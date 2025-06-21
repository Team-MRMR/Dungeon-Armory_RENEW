// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Stay.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_ARMORY_API UBTTask_Stay : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

/***** Variables *****/
	
/***** Functions *****/
public:
	UBTTask_Stay();

	UFUNCTION()
	void OnStayCompleted(UBehaviorTreeComponent* BehaviorTreeComponent);

protected:
    /** 태스크 실행 */
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


};
