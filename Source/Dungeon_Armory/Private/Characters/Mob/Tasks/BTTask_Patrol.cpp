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
	bNotifyTick = true;

	NodeName = TEXT("Mob Patrol");
}

void UBTTask_Patrol::InitializeFromAsset(UBehaviorTree& BehaviorTreeAsset)
{
	Super::InitializeFromAsset(BehaviorTreeAsset);
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		return EBTNodeResult::Failed;
	}

	auto Stat = Cast<UCharacterStatComponent>(Blackboard->GetValueAsObject(MobBBKeys::Stat));
	if (!Stat)
	{
		return EBTNodeResult::Failed;
	}

	auto* NavSys = UNavigationSystemV1::GetCurrent(OwnerComp.GetWorld());
	if (!NavSys)
	{
		return EBTNodeResult::Failed;
	}

	FVector HomeLocation = Blackboard->GetValueAsVector(MobBBKeys::HomeLocation);

	FNavLocation RandomLocation;
	bool bFound = NavSys->GetRandomPointInNavigableRadius(HomeLocation, Stat->PatrolRadius, RandomLocation);
	if (!bFound)
	{
		return EBTNodeResult::Failed;
	}
	Blackboard->SetValueAsVector(MobBBKeys::RandomLocation, RandomLocation.Location);

	// MovementControllerComponent 할당 과정
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	AMobBase* MobBase = Cast<AMobBase>(Pawn);
	if (!MobBase)
	{
		return EBTNodeResult::Failed;
	}

	auto MovementController = MobBase->FindComponentByClass<UMovementControllerComponent>();
	if (!MovementController)
	{
		return EBTNodeResult::Failed;
	}
	Blackboard->SetValueAsObject(MobBBKeys::MovementController, MovementController);

	return EBTNodeResult::InProgress;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		return;
	}

	auto* MovementController = Cast<UMovementControllerComponent>(Blackboard->GetValueAsObject(MobBBKeys::MovementController));
	if (!MovementController)
	{
		return;
	}

	auto* Stat = Cast<UCharacterStatComponent>(Blackboard->GetValueAsObject(MobBBKeys::Stat));
	if (!Stat)
	{
		return;
	}

	FVector Location = Blackboard->GetValueAsVector(MobBBKeys::RandomLocation);
	MovementController->MoveToDestination(Location);
}

void UBTTask_Patrol::OnMoveCompleted(UBehaviorTreeComponent* BTComp)
{
	// Task 종료
	FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
}