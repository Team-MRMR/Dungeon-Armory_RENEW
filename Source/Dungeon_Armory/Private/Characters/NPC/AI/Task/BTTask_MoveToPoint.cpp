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

	// 이 태스크가 Vector 타입 키를 요구하도록 설정
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveToPoint, BlackboardKey));
}

EBTNodeResult::Type UBTTask_MoveToPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return EBTNodeResult::Failed;

	FVector Point = Blackboard->GetValueAsVector(BlackboardKey.SelectedKeyName);

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(Point);
	MoveRequest.SetAcceptanceRadius(10.f);

	FNavPathSharedPtr NavPath;
	FPathFollowingRequestResult Result = AIController->MoveTo(MoveRequest, &NavPath);

	if (Result.Code == EPathFollowingRequestResult::Failed)
		return EBTNodeResult::Failed;

	if (Result.Code == EPathFollowingRequestResult::AlreadyAtGoal)
		return EBTNodeResult::Succeeded;

	return EBTNodeResult::InProgress;
}

void UBTTask_MoveToPoint::OnMoveCompleted(UBehaviorTreeComponent* BehaviorTreeComponent)
{
	FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
}