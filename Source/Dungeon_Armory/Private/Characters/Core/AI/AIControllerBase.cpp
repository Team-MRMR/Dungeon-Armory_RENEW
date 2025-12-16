// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Core/AI/AIControllerBase.h"
#include "Characters/Core/AI/Interface/IMovableTask.h"
#include "Characters/Core/AI/Team/TeamComponent.h"
#include "Characters/NPC/NPCBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Manager/TeamManager.h"


#include "Navigation/PathFollowingComponent.h"

AAIControllerBase::AAIControllerBase()
{
    PrimaryActorTick.bCanEverTick = false;

	// 팀 컴포넌트 생성 -> 하위 클래스에서 팀 설정 필수
	TeamComponent = CreateDefaultSubobject<UTeamComponent>(TEXT("TeamComponent"));
    
    // AI 감지 시스템 초기화
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// 비헤이비어 트리 컴포넌트 초기화
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AAIControllerBase::BeginPlay()
{
    Super::BeginPlay();
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

ETeamAttitude::Type AAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
    if (!TeamComponent)
    {
	    return ETeamAttitude::Neutral;
    }

	return TeamComponent->GetTeamAttitudeTowards(Other);
}

FGenericTeamId AAIControllerBase::GetGenericTeamId() const
{
    if (!TeamComponent)
    {
        return FGenericTeamId::NoTeam;
    }

    return TeamComponent->GetGenericTeamId();
}

void AAIControllerBase::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    if (!BehaviorTreeComponent)
        return;

    const UBTNode* ActiveNode = BehaviorTreeComponent->GetActiveNode();
    if (!ActiveNode)
        return;

    // IMovableTask 인터페이스를 사용하여 이동 완료 처리
    if (IMovableTask* MovableTask = Cast<IMovableTask>(const_cast<UBTNode*>(ActiveNode)))
    {
        MovableTask->OnMoveCompleted(BehaviorTreeComponent);

        if (Result != EPathFollowingResult::Success)
        {
            BehaviorTreeComponent->RestartTree();
        }
    }
}