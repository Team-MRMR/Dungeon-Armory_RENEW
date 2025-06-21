// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/NPC/AI/Task/BTTask_RoamToPoints.h"
#include "Characters/NPC/NPCBase.h"

#include "Characters/Core/AI/AIControllerBase.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "NavigationSystem.h"

UBTTask_RoamToPoints::UBTTask_RoamToPoints()
{
    bNotifyTick = false;
    NodeName = "RoamToPoints"; // BT에서 보이는 이름
}

EBTNodeResult::Type UBTTask_RoamToPoints::ExecuteTask(UBehaviorTreeComponent& OwnerCmp, uint8* NodeMemory)
{
    auto NPCController = Cast<AAIControllerBase>(OwnerCmp.GetAIOwner());
    if (!NPCController)
    {
        return EBTNodeResult::Failed;
    }

	auto* NPC = Cast<ANPCBase>(NPCController->GetPawn());
	if (!NPC)
	{
		return EBTNodeResult::Failed;
	}

    // 이동 명령 실행
    FVector MovePointLocation = NPC->GetNextPoint();
    NPCController->MoveToLocation(MovePointLocation);

    // 이동 완료 후 OnRoamToPointReached에서 처리, OnRoamingReached 호출

    return EBTNodeResult::InProgress;
}

//void OnMoveCompleted(UBehaviorTreeComponent* OwnerComp, EBTNodeResult::Type Result = EBTNodeResult::Succeeded)
//{
//    FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
//}
