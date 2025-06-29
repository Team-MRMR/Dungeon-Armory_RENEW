// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/AI/NPCAIController.h"
#include "Characters/NPC/NPCBase.h"

#include "Characters/Core/Component/CharacterStatComponent.h"
#include "Characters/Core/Component/MovementControllerComponent.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Navigation/PathFollowingComponent.h"

ANPCAIController::ANPCAIController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANPCAIController::BeginPlay()
{
    Super::BeginPlay();

}

void ANPCAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // 컴포넌트 참조 할당
    if (ACharacter* NPCCharacter = Cast<ANPCBase>(InPawn))
    {
        if (ANPCBase* NPCBase = Cast<ANPCBase>(NPCCharacter))
        {
			Stat = NPCBase->FindComponentByClass<UCharacterStatComponent>();

            MovementController = NPCBase->FindComponentByClass<UMovementControllerComponent>();
        }
    }

    // 비헤이비어 트리 실행
    if (BehaviorTree && BehaviorTree->BlackboardAsset)
    {
        // 블랙보드 설정 (AIController에서 관리됨)
        UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent);

        // 블랙보드 변수 초기화
        if (BlackboardComponent)
        {
            InitializeBlackboardKeys();
        }

        // 비헤이비어 트리 실행 (이 시점에서 BehaviorTreeComponent가 자동으로 생성됨)
        RunBehaviorTree(BehaviorTree);

        // 내부적으로 생성된 BehaviorTreeComponent를 가져와서 멤버 변수에 할당
        BehaviorTreeComponent = FindComponentByClass<UBehaviorTreeComponent>();
    }
}

void ANPCAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ANPCAIController::InitializeBlackboardKeys()
{
    //// --- 상태 관련 키값 ---
    //BlackboardComponent->SetValueAsEnum(NPCBBKeys::NPCState, static_cast<uint8>(ENPCStates::Stay));

    //// --- 컴포넌트 관련 키값 ---
    //BlackboardComponent->SetValueAsObject(NPCBBKeys::Stat, Stat);
    //BlackboardComponent->SetValueAsObject(NPCBBKeys::MovementController, MovementController);

    //// --- 거리 관련 키값 ---
    //BlackboardComponent->SetValueAsVector(NPCBBKeys::HomeLocation, GetPawn()->GetActorLocation());
}

void ANPCAIController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	Super::OnMoveCompleted(RequestID, Result);
}