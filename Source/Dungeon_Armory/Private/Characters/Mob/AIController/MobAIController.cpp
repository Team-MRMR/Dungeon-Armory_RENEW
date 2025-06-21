// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mob/AIController/MobAIController.h"
#include "Characters/Mob/Component/MobAttackComponent.h"
#include "Characters/Mob/MobBase.h"

#include "Characters/Core/Component/CharacterStatComponent.h"
#include "Characters/Core/Component/MovementControllerComponent.h"

#include "Characters/Core/AI/Interface/IMovableTask.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

const FName AMobAIController::MobStateKey(TEXT("MobState"));

AMobAIController::AMobAIController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMobAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AMobAIController::OnPossess(APawn* InPawn)
{
    // 팀 컴포넌트 설정
    Super::OnPossess(InPawn);

    // 컴포넌트 참조 할당
    if (ACharacter* MobCharacter = Cast<ACharacter>(InPawn))
    {
        if (AMobBase* MobBase = Cast<AMobBase>(MobCharacter))
        {
            StatComponent = MobBase->FindComponentByClass<UCharacterStatComponent>();

			MovementControllerComponent = MobBase->FindComponentByClass<UMovementControllerComponent>();
			MovementControllerComponent->OnMovementCompleted.AddDynamic(this, &AMobAIController::OnMovementCompleted);

			MobAttackComponent = MobBase->FindComponentByClass<UMobAttackComponent>();
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

    if (StatComponent)
    {
        if (SightConfig)
        {
            SightConfig->SightRadius = StatComponent->SightRadius;
            SightConfig->LoseSightRadius = StatComponent->LoseSightRadius;
            SightConfig->PeripheralVisionAngleDegrees = StatComponent->PeripheralVisionAngleDegrees;
            SightConfig->DetectionByAffiliation.bDetectEnemies = true;
            SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
            SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

            if (AIPerception)
            {
                // Perception 컴포넌트에 시야 감지 설정 추가
                AIPerception->ConfigureSense(*SightConfig);

                // 우선순위가 가장 높은 감각으로 설정
                AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

                // 감지 이벤트 콜백 등록
                AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMobAIController::OnTargetPerceived);
            }
        }
    }
}

void AMobAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!DetectedPlayer)
        return;

    // 플레이어와의 거리 계산
    FVector MobLocation = GetPawn()->GetActorLocation();
    FVector PlayerLocation = DetectedPlayer->GetActorLocation();
    float Distance = FVector::Dist2D(MobLocation, PlayerLocation);

    if (Distance <= StatComponent->AttackableDistance)  // 공격 범위 내라면
    {
		SetMobState(EMobState::Battle);
    }
    else if (Distance <= StatComponent->SightRadius)    // 추격 범위 내라면
    {
		SetMobState(EMobState::Chase);
    }
}

void AMobAIController::SetMobState(EMobState NewState)
{
    EMobState CurrState = static_cast<EMobState>(BlackboardComponent->GetValueAsEnum(MobStateKey));
    if (CurrState == NewState)
    {
        return;  // 동일 상태로의 변경은 무시
    }

    // 블랙보드에 상태 업데이트
    if (BlackboardComponent)
    {
        BlackboardComponent->SetValueAsEnum(MobStateKey, static_cast<uint8>(NewState));
    }
}

void AMobAIController::InitializeBlackboardKeys()
{
	// --- 상태 관련 키값 ---S
    BlackboardComponent->SetValueAsEnum(MobBBKeys::MobState, static_cast<uint8>(EMobState::Patrol));

	// --- 컴포넌트 관련 키값 ---
	BlackboardComponent->SetValueAsObject(MobBBKeys::Stat, StatComponent);
	BlackboardComponent->SetValueAsObject(MobBBKeys::MovementController, MovementControllerComponent);
	BlackboardComponent->SetValueAsObject(MobBBKeys::AttackComponent, MobAttackComponent);

    // --- 거리 관련 키값 ---
    BlackboardComponent->SetValueAsVector(MobBBKeys::HomeLocation, GetPawn()->GetActorLocation());
}

void AMobAIController::OnMovementCompleted()
{
    if (!BehaviorTreeComponent)
        return;

    const UBTNode* ActiveNode = (BehaviorTreeComponent->GetActiveNode());
    if (ActiveNode)
    {
        // IIMovableTask 인터페이스를 사용하여 이동 완료 처리
        IIMovableTask* MovableTask = const_cast<IIMovableTask*>(Cast<IIMovableTask>(ActiveNode));
        if (MovableTask)
        {
            MovableTask->OnMoveCompleted(BehaviorTreeComponent);
        }
    }
}

// 감지 이벤트 처리
void AMobAIController::OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus)
{
    if (!Actor)
        return;

    if (!BlackboardComponent)
        return;

    bool bDetectedTarget = Stimulus.WasSuccessfullySensed();
    if (bDetectedTarget)
    {
        // 현재 실행 중인 BTTask 중단
        BehaviorTreeComponent->RequestExecution(EBTNodeResult::Type::Aborted);
        //OnMovementCompleted();

        // 플레이어가 감지됨
        DetectedPlayer = Actor;
        BlackboardComponent->SetValueAsObject("Target", Actor);
    }
    else if(DetectedPlayer == Actor)
    {
        // 현재 실행 중인 BTTask 중단
        BehaviorTreeComponent->RequestExecution(EBTNodeResult::Type::Aborted);
        //OnMovementCompleted();

        // 플레이어를 놓침
        DetectedPlayer = nullptr;
        BlackboardComponent->SetValueAsObject("Target", nullptr);
        SetMobState(EMobState::Patrol);
    }
}
