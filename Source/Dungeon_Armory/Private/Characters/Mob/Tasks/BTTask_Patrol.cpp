// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mob/Tasks/BTTask_Patrol.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/Mob/MobBase.h"
#include "Characters/Mob/AIController/MobAIController.h"
#include "Characters/Core/Component/CharacterStatComponent.h"
#include "Characters/Core/Component/MovementControllerComponent.h"

#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = TEXT("Mob Patrol");
	bNotifyTick = false;
}

void UBTTask_Patrol::InitializeFromAsset(UBehaviorTree& BehaviorTreeAsset)
{
	Super::InitializeFromAsset(BehaviorTreeAsset);
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return EBTNodeResult::Failed;

	UCharacterStatComponent* Stat = Cast<UCharacterStatComponent>(Blackboard->GetValueAsObject(BBKeys::Mob::Stat));
	if (!Stat)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(OwnerComp.GetWorld());
	if (!NavSys)
		return EBTNodeResult::Failed;

	FVector HomeLocation = Blackboard->GetValueAsVector(BBKeys::Mob::HomeLocation);

	FNavLocation RandomLocation;
	bool bFound = NavSys->GetRandomPointInNavigableRadius(HomeLocation, Stat->PatrolRadius, RandomLocation);
	if (!bFound)
		return EBTNodeResult::Failed;

	Blackboard->SetValueAsVector(BBKeys::Mob::RandomLocation, RandomLocation.Location);

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(RandomLocation);
	MoveRequest.SetAcceptanceRadius(10.f);

	FNavPathSharedPtr NavPath;
	FPathFollowingRequestResult Result = AIController->MoveTo(MoveRequest, &NavPath);

	if (Result.Code == EPathFollowingRequestResult::Failed)
	{
		return EBTNodeResult::Failed; // 이동 실패 시 즉시 실패 반환
	}

	if (Result.Code == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		return EBTNodeResult::Succeeded; // 이미 도착했다면 즉시 성공 반환
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Patrol::OnMoveCompleted(UBehaviorTreeComponent* BTComp)
{
	// Task 종료
	FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
}