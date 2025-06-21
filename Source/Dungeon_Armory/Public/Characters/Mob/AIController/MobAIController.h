// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Core/AI/AIControllerBase.h"
#include "Characters/Core/Component/CharacterStatComponent.h"

#include "MobAIController.generated.h"

class UCharacterStatComponent;
class UMovementControllerComponent;
class UMobAttackComponent;

UCLASS()
class DUNGEON_ARMORY_API AMobAIController : public AAIControllerBase
{
	GENERATED_BODY()
	
/***** Unreal *****/
public:
	AMobAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

/***** Component *****/
private:
	UCharacterStatComponent* StatComponent;
	UMovementControllerComponent* MovementControllerComponent;
	UMobAttackComponent* MobAttackComponent;

/***** Behavior Tree *****/
protected:
	// 블랙보드 키 이름 상수
	static const FName MobStateKey;

public:
	void SetMobState(EMobState NewState);

private:
	void InitializeBlackboardKeys();

	UFUNCTION()
	void OnMovementCompleted();

/***** AI *****/
protected:
	UFUNCTION()
	void OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus) override;
};

// --- 블랙보드 키값 스트링 캐싱 ---
namespace MobBBKeys
{
	// 상태 수치
	static const FName MobState(TEXT("MobState"));

	// 플레이어 객체
	static const FName Target(TEXT("Target"));

	// 스탯 관련 수치
	static const FName Stat(TEXT("Stat"));

	// 이동 관련 객체
	static const FName MovementController(TEXT("MovementController"));

	// 공격 관련 객체
	static const FName AttackComponent(TEXT("AttackComponent"));

	// 거리 관련 수치
	static const FName HomeLocation(TEXT("HomeLocation"));
	static const FName RandomLocation(TEXT("RandomLocation"));
}