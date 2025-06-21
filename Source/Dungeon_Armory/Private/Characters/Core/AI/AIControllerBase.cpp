// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Core/AI/AIControllerBase.h"
#include "Characters/NPC/NPCBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Manager/TeamManager.h"

#include "Characters/Core/AI/Team/TeamComponent.h"

AAIControllerBase::AAIControllerBase()
{
    PrimaryActorTick.bCanEverTick = false;

    // AI 감지 시스템 초기화
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    // 팀 컴포넌트 생성
	TeamComponent = CreateDefaultSubobject<UTeamComponent>(TEXT("TeamComponent"));

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

    if (ANPCBase* NPC = Cast<ANPCBase>(InPawn))
    {
        // TeamComponent에서 팀 정보를 가져옴
        if (UTeamComponent* TeamCmp = NPC->TeamComponent)
        {
            TeamComponent = TeamCmp; // 팀 정보를 가져와 저장
        }
    }
}

ETeamAttitude::Type AAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
    if (TeamComponent)
    {
		return TeamComponent->GetTeamAttitudeTowards(Other);
    }

	return ETeamAttitude::Neutral;
}

FGenericTeamId AAIControllerBase::GetGenericTeamId() const
{
    if (TeamComponent)
    {
        return TeamComponent->GetGenericTeamId();
    }

    return FGenericTeamId::NoTeam;
}