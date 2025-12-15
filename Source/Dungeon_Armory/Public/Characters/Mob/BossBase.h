// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/Mob/MobBase.h"

#include "BossBase.generated.h"

class AMobBase;

class UCharacterStatComponent;
class UBossAttackComponent;
class UMovementControllerComponent;
class UTeamComponent;
class UBlackboardComponent;

class AAIController;

class USoundBase;

UCLASS()
class DUNGEON_ARMORY_API ABossBase : public AMobBase
{
	GENERATED_BODY()

/***** Unreal *****/
public:
	ABossBase();

protected:
	virtual void BeginPlay() override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

// ----- BossBase
protected:
	 void CreateAttackComponent(UAttackComponentBase* NewAttackComponent);
	 //void CreateAttackComponent(UAttackComponentBase* NewAttackComponent);
	 //virtual void CreateAttackComponent_Implementation(UAttackComponentBase* NewAttackComponent) override { /*Empty Body*/ };
};
