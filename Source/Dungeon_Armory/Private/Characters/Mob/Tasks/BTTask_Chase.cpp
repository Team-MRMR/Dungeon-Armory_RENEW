// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mob/Tasks/BTTask_Chase.h"
#include "Characters/Mob/MobBase.h"
#include "Characters/Mob/AIController/MobAIController.h"

#include "Characters/Core/Component/MovementControllerComponent.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Navigation/PathFollowingComponent.h"

UBTTask_Chase::UBTTask_Chase()
{
    bNotifyTick = true;
    NodeName = TEXT("Mob Chase");
}

void UBTTask_Chase::InitializeFromAsset(UBehaviorTree& BehaviorTreeAsset)
{
    Super::InitializeFromAsset(BehaviorTreeAsset);
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::InProgress;
}

void UBTTask_Chase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard)
    {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(BBKeys::Mob::Target));
	if (!TargetActor)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(TargetActor->GetTargetLocation());
	MoveRequest.SetAcceptanceRadius(10.f);

	FNavPathSharedPtr NavPath;
	FPathFollowingRequestResult Result = AIController->MoveTo(MoveRequest, &NavPath);
}

void UBTTask_Chase::OnMoveCompleted(UBehaviorTreeComponent* BTComp)
{
    // Task Á¾·á
    FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
}