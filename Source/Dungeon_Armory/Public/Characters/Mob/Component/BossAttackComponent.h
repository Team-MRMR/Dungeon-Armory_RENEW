// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/Mob/BossBase.h"
#include "Characters/Mob/Component/MobAttackComponent.h"
#include "BossAttackComponent.generated.h"

class AMob;
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

// ---- Boss Skill - logic
private:
	bool bIsTimeDropRock;

private:


private:
	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RoarMontage;

// ----- Boss Skill - DropRock
private:
	UPROPERTY(EditDefaultsOnly, Category = "Skill | DropRock", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> RockClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill | DropRock", meta = (AllowPrivateAccess = "true"))
	float DropRockSpawnHeight = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill | DropRock", meta = (AllowPrivateAccess = "true"))
	float DropRockSpawnRadius = 500.f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill | DropRock", meta = (AllowPrivateAccess = "true"))
	int DropRockNumber = 5;

	bool bIsDroppingRocks = false;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "BossSkill")
	void DropRockSkill();
	void DropRockSkill_Implementation();

	void SpawnRock(const FVector& Location);

	UFUNCTION()
	virtual void OnDropRockAnimationEnd(UAnimMontage* Montage, bool bInterrupted);

private:
	FVector GetRandomPointInDropRockSpawnRadius();

// ----- Boss Skill - SpawnMob
private:
	UPROPERTY(EditDefaultsOnly, Category = "Skill | SpawnMob", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AMob>> SpawnMobList;

	UPROPERTY(EditDefaultsOnly, Category = "Skill | SpawnMob", meta = (AllowPrivateAccess = "true"))
	float MobSpawnHeight = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill | SpawnMob", meta = (AllowPrivateAccess = "true"))
	float MobSpawnRadius = 500.f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill | SpawnMob", meta = (AllowPrivateAccess = "true"))
	int SpawnMobNumber = 5;

	bool bIsSpawningMobs = false;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "BossSkill")
	void SpawnMobSkill();
	void SpawnMobSkill_Implementation();

	void SpawnMob(const FVector& Location);

	UFUNCTION()
	virtual void OnSpawnMobAnimationEnd(UAnimMontage* Montage, bool bInterrupted);

private:
	FVector GetRandomPointInMobSpawnRadius();

};
