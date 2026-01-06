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

	UE_LOG(LogTemp, Warning, TEXT("HomeLocation - RandomLocation: %f."), FVector::Distance(HomeLocation, RandomLocation));

	Blackboard->SetValueAsVector(BBKeys::Mob::RandomLocation, RandomLocation.Location);

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(RandomLocation);
	MoveRequest.SetAcceptanceRadius(10.f);

	FNavPathSharedPtr NavPath;
	FPathFollowingRequestResult Result = AIController->MoveTo(MoveRequest, &NavPath);
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Patrol::OnMoveCompleted(UBehaviorTreeComponent* BTComp)
{
	// Task Á¾·á
	FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
}