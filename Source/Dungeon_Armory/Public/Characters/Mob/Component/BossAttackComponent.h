// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/Mob/BossBase.h"
#include "Characters/Mob/Component/MobAttackComponent.h"
#include "BossAttackComponent.generated.h"

class UAnimMontage;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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
private:
	ABossBase* BossOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int SkillCycleByCount = 4;

	int AttackCount = 0;

public:
	int GetAttackCount() const { return AttackCount; }
	void IncrementAttackCount() { ++AttackCount; }
	void ResetAttackCount() { AttackCount = 0; }

// ----- Boss Skill
private:
	UPROPERTY(EditDefaultsOnly, Category = "Skill | DropRock", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> RockClass;
	UPROPERTY(EditDefaultsOnly, Category = "Skill | DropRock", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> RockIndicatorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill | DropRock", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RoarMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Skill | DropRock", meta = (AllowPrivateAccess = "true"))
	float DropRockHeight = 1000.f;
	float DropRockRadius = 500.f;
	float DropRockNumber = 5.f;

	float RockIndicatorDuration = 1.5f;

	bool bIsDroppingRocks = false;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "BossSkill")
	void DropRockSkill();
	void DropRockSkill_Implementation();

	void SpawnRock(const FVector& Location);

	FVector GetRandomPointInRadius();

	UFUNCTION()
	virtual void OnDropRockAnimationEnd(UAnimMontage* Montage, bool bInterrupted);
};
