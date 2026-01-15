// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Stun.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_ARMORY_API UBTTask_Stun : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_Stun();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	void OnStunFinished(UBehaviorTreeComponent* OwnerComp);

	UPROPERTY(EditAnywhere, Category = "Stun")
	float StunDuration = 3.0f;
};
