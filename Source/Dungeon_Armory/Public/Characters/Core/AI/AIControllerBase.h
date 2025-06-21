// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "AIControllerBase.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UTeamComponent;
class UBehaviorTreeComponent;

struct FAIStimulus;

/**
 * Base Class에서 Controller의 기능적 역할을 맡고,
 * Dervied Class에서는 비헤이비어 트리와 관련된 작업을 실행하는 역할을 수행
 */
UCLASS()
class DUNGEON_ARMORY_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

/***** Unreal *****/
public:
	AAIControllerBase();

protected:
	virtual void BeginPlay() override;				// Called when the game starts or when spawned
	virtual void OnPossess(APawn* InPawn) override;	// Called when the controller possess a Pawn

/***** AI *****/
protected:
	// 현재 감지 중인 플레이어 참조
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	AActor* DetectedPlayer;

	// AI 감지 시스템
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionComponent* AIPerception;

	// AI 시각 감지 설정
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

protected:
	// 감지 처리 함수
	virtual void OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus) { };

/***** Team *****/
protected:
	// 팀 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Team")
	UTeamComponent* TeamComponent;

    // AI 시각 감지 설정
    UPROPERTY(VisibleAnywhere, Category = "Team")
	FGenericTeamId TeamId;

public:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	
/***** Behavior Tree *****/
public:
	/** 비헤이비어 트리 컴포넌트 */
	UPROPERTY(VisibleAnywhere, Category = "Behavior Tree")
	UBehaviorTreeComponent* BehaviorTreeComponent;

	/** 블랙보드 컴포넌트 */
	UPROPERTY(VisibleAnywhere, Category = "Behavior Tree")
	UBlackboardComponent* BlackboardComponent;

protected:
	/** 비헤이비어 트리 */
	UPROPERTY(EditDefaultsOnly, Category = "Behavior Tree")
	UBehaviorTree* BehaviorTree;

	/** 블랙보드에서 NPC의 상태를 저장하는 키 */
	UPROPERTY(EditDefaultsOnly, Category = "Behavior Tree")
	FName BBKey_NPCState = "NPCState";
};
