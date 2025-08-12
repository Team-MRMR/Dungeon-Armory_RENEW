// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Core/AI/AIControllerBase.h"

#include "NPCAIController.generated.h"

class UCharacterStatComponent;
class UMovementControllerComponent;

class ANPCBase;

namespace BBKeys
{
	namespace NPC
	{
		// 블랙보드 키 이름 상수
		static const FName NPCState(TEXT("NPCState"));

		static const FName IsShopping(TEXT("IsShopping"));

		static const FName PayPoint(TEXT("PayPoint"));
		static const FName ExitPoint(TEXT("ExitPoint"));
		static const FName ShoppingPoint(TEXT("ShoppingPoint"));

		static const FName ReturnPoint(TEXT("ReturnPoint"));
		static const FName RoammingPoint(TEXT("RoammingPoint"));
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
	virtual void InitializeBlackboardKeys(ANPCBase* NPCBase);
};