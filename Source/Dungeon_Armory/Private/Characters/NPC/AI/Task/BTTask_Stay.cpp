// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/AI/Task/BTTask_Stay.h"

#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Stay::UBTTask_Stay()
{
	NodeName = "NPC Stay";
}

EBTNodeResult::Type UBTTask_Stay::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

// 컨트롤러에서 호출
void UBTTask_Stay::OnStayCompleted(UBehaviorTreeComponent* BehaviorTreeComponent)
{
    FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
}
