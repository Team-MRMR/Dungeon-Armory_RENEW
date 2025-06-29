// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Core/AI/AIControllerBase.h"

#include "NPCAIController.generated.h"

class UCharacterStatComponent;
class UMovementControllerComponent;

namespace BBKeys
{
	namespace NPC
	{
		// 블랙보드 키 이름 상수
		static const FName LocationPoint(TEXT("LocationPoint"));
		static const FName IsShopping(TEXT("IsShopping"));
	}
}

UCLASS()
class DUNGEON_ARMORY_API ANPCAIController : public AAIControllerBase
{
	GENERATED_BODY()

/***** Functions (Unreal) *****/
public:
	ANPCAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) override;


/***** Component *****/
private:
	UCharacterStatComponent* Stat;
	UMovementControllerComponent* MovementController;

/***** Functions (Task) *****/
private:
	void InitializeBlackboardKeys();
};