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
    bNotifyTick = false;
    NodeName = TEXT("Mob Chase");
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!AIController || !Blackboard)
        return EBTNodeResult::Failed;

    AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(BBKeys::Mob::Target));
    if (!TargetActor) return EBTNodeResult::Failed;

    // MoveToActor를 사용하면 타겟이 움직여도 엔진이 자동으로 추적합니다.
    // bAllowRebind를 true로 설정하면 타겟 위치 변화에 대응합니다.
    FAIMoveRequest MoveRequest(TargetActor);
    MoveRequest.SetAcceptanceRadius(10.f);
    MoveRequest.SetCanStrafe(true);

    EPathFollowingRequestResult::Type RequestResult = AIController->MoveToActor(TargetActor, 10.f);

    if (RequestResult == EPathFollowingRequestResult::Failed)
    {
        return EBTNodeResult::Failed;
    }

    // 이미 도착한 상태라면 성공 반환
    if (RequestResult == EPathFollowingRequestResult::AlreadyAtGoal)
    {
        return EBTNodeResult::Succeeded;
    }

    // 이동 중임을 나타냄 (OnMoveCompleted가 호출될 때까지 대기)
    return EBTNodeResult::InProgress;
}

void UBTTask_Chase::OnMoveCompleted(UBehaviorTreeComponent* BTComp)
{
    // Task 종료
    FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
}