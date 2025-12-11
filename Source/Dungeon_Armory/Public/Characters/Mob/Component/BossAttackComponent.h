// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/Mob/BossBase.h"
#include "Characters/Mob/Component/MobAttackComponent.h"
#include "BossAttackComponent.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_ARMORY_API UBossAttackComponent : public UMobAttackComponent
{
	GENERATED_BODY()

// ----- Unreal
public:
	UBossAttackComponent();

protected:
	virtual void BeginPlay() override;

// ----- Parent
public:
	void StartAttack() override;

// ----- BossAttackComponent
public:
	int GetAttackCount() const { return AttackCount; }
	void IncrementAttackCount() { ++AttackCount; }
	void ResetAttackCount() { AttackCount = 0; }

protected:
	UFUNCTION(BlueprintNativeEvent = "Boss Skill", meta = (AllowPrivateAccess = "true"))
	void DropRockSkill();
	void DropRockSkill_Implementation() {/*body*/};


private:
	ABossBase* BossOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int SkillCycleByCount = 4;

	int AttackCount = 0;

};
