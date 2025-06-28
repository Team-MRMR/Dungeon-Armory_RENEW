// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/AI/Task/BTTask_MoveToPoint.h"
#include "Characters/NPC/AI/LocationPoint.h"
#include "Characters/NPC/AI/NPCAIController.h"
#include "Characters/NPC/NPCBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"


UBTTask_MoveToPoint::UBTTask_MoveToPoint()
{
	bNotifyTick = false;
    NodeName = "Move To Point"; // BT에서 보이는 이름
}

EBTNodeResult::Type UBTTask_MoveToPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return EBTNodeResult::Failed;

	ANPCBase* NPC = Cast<ANPCBase>(AIController->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return EBTNodeResult::Failed;

	ALocationPoint* LocationPoint = Cast<ALocationPoint>(Blackboard->GetValueAsObject(BBKeys::LocationPoint));
	if (!LocationPoint)
		return EBTNodeResult::Failed;

	FVector Location = LocationPoint->GetActorLocation();

	EPathFollowingRequestResult::Type Result = AIController->MoveToLocation(Location, 50.f);

	if (Result == EPathFollowingRequestResult::Failed)
		return EBTNodeResult::Failed;

	if (Result == EPathFollowingRequestResult::AlreadyAtGoal)
		return EBTNodeResult::Succeeded;

	return EBTNodeResult::InProgress;
}