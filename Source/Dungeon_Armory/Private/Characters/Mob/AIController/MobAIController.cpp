// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mob/AIController/MobAIController.h"
#include "Characters/Mob/Component/MobAttackComponent.h"
#include "Characters/Mob/MobBase.h"

#include "Characters/Core/AI/Team/TeamComponent.h"
#include "Characters/Core/Component/CharacterStatComponent.h"
#include "Characters/Core/AI/Interface/IMovableTask.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/Core/AI/Interface/IMovableTask.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

const FName AMobAIController::MobStateKey(TEXT("MobState"));

AMobAIController::AMobAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    TeamComponent->SetTeamType(ETeamType::Mob);
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
    AMobBase* MobBase = Cast<AMobBase>(InPawn);
    if (MobBase)
    {
        StatComponent = MobBase->StatComponent;
        MobAttackComponent = Cast<UMobAttackComponent>(MobBase->AttackComponent);
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

    InitializePerceptionSystem();
}

void AMobAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!GetPawn())
    {
        return;
    }

    if (GetMobState() == EMobState::Dead)
        return;

    if (!DetectedPlayer)
    {
        SetMobState(EMobState::Patrol);
        return;
    }

    // --- 위치 계산 ---
    FVector MobLocation = GetPawn()->GetActorLocation();
    FVector PlayerLocation = DetectedPlayer->GetActorLocation();

    // --- 몬스터가 바라보는 방향 ---
    FVector Forward = GetPawn()->GetActorForwardVector().GetSafeNormal2D();

    // --- 몬스터에서 플레이어까지의 방향 ---
    FVector ToPlayer = (PlayerLocation - MobLocation).GetSafeNormal2D();

    // --- 시야각 계산 ---
    float Dot = FVector::DotProduct(Forward, ToPlayer);
    float AngleDeg = FMath::Acos(Dot) * (180.0f / PI);

    float HalfVision = StatComponent->PeripheralVisionAngleDegrees * 0.5f;

    // --- 시야각 판정 ---
    EMobState MobState = EMobState::None;
    if (AngleDeg <= HalfVision)
    {
        // --- 거리 계산 ---
        float Distance = FVector::Distance(MobLocation, PlayerLocation);

        // --- 거리 기반 상태 전이 ---
        if (Distance <= StatComponent->AttackableDistance)  // 공격 범위 내라면
        {
            MobState = EMobState::Battle;
        }
        else
        {
            MobState = EMobState::Chase;
        }
    }
    else
    {
        MobState = EMobState::Chase;
    }

    SetMobState(MobState);
    StatComponent->SetSpeedForState(MobState);
}

EMobState AMobAIController::GetMobState() const
{
    if (!BlackboardComponent)
        return EMobState::None;

	return static_cast<EMobState>(BlackboardComponent->GetValueAsEnum(MobStateKey));
}

void AMobAIController::SetMobState(EMobState NewState)
{
    if (!BlackboardComponent)
        return;

    EMobState CurrState = GetMobState();
    if (CurrState != NewState)
    {
        UE_LOG(LogTemp, Warning, TEXT("@@@ State Changed: %d -> %d"), (int)CurrState, (int)NewState);
        // 동일 상태로의 변경은 무시
        BlackboardComponent->SetValueAsEnum(MobStateKey, static_cast<uint8>(NewState));
    }
}

void AMobAIController::InitializeBlackboardKeys()
{
	// --- 상태 관련 키값 ---
    BlackboardComponent->SetValueAsEnum(BBKeys::Mob::MobState, static_cast<uint8>(EMobState::Patrol));

	// --- 컴포넌트 관련 키값 ---
	BlackboardComponent->SetValueAsObject(BBKeys::Mob::Stat, StatComponent);
	//BlackboardComponent->SetValueAsObject(BBKeys::Mob::MovementController, MovementControllerComponent);
	BlackboardComponent->SetValueAsObject(BBKeys::Mob::AttackComponent, MobAttackComponent);

    // --- 거리 관련 키값 ---
    BlackboardComponent->SetValueAsVector(BBKeys::Mob::HomeLocation, GetPawn()->GetActorLocation());
}

void AMobAIController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	Super::OnMoveCompleted(RequestID, Result);
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

        // 플레이어가 감지됨
        DetectedPlayer = Actor;
        BlackboardComponent->SetValueAsObject(BBKeys::Mob::Target, Actor);
    }
    else if (DetectedPlayer == Actor)
    {
        // 현재 실행 중인 BTTask 중단
        BehaviorTreeComponent->RequestExecution(EBTNodeResult::Type::Aborted);

        // 플레이어를 놓침
        DetectedPlayer = nullptr;
        BlackboardComponent->SetValueAsObject(BBKeys::Mob::Target, nullptr);
        ResetPerceptionRadius();
    }
}

void AMobAIController::InitializePerceptionSystem()
{
    if (!StatComponent || !SightConfig || !AIPerception)
        return;

    SightConfig->SightRadius = StatComponent->SightRadius;
    SightConfig->LoseSightRadius = StatComponent->LoseSightRadius;
    SightConfig->PeripheralVisionAngleDegrees = StatComponent->PeripheralVisionAngleDegrees;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

    // Perception 컴포넌트에 시야 감지 설정 추가
    AIPerception->ConfigureSense(*SightConfig);

    // 우선순위가 가장 높은 감각으로 설정
    AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

    // 감지 이벤트 콜백 등록
    AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMobAIController::OnTargetPerceived);
}

void AMobAIController::ResetPerceptionRadius()
{
    if (!SightConfig || !StatComponent)
        return;

    SightConfig->PeripheralVisionAngleDegrees = StatComponent->PeripheralVisionAngleDegrees;

    AIPerception->ConfigureSense(*SightConfig);
    AIPerception->RequestStimuliListenerUpdate();
}

void AMobAIController::ExtentdPerceptionRadius()
{
    if (!SightConfig)
        return;

    SightConfig->PeripheralVisionAngleDegrees = 360.f;

    AIPerception->ConfigureSense(*SightConfig);
    AIPerception->RequestStimuliListenerUpdate();
}
