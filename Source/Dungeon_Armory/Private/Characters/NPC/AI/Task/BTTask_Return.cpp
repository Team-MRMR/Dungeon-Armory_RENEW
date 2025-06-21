// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/AI/Task/BTTask_Return.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_Return::UBTTask_Return()
{
    NodeName = "Return"; // BT에서 보이는 이름
}

EBTNodeResult::Type UBTTask_Return::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    FVector HomeLocation = BlackboardComp->GetValueAsVector("HomeLocation");
    AIController->MoveToLocation(HomeLocation);

    return EBTNodeResult::Succeeded;
}