// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/Mob/Mob.h"
#include "Characters/Mob/BossBase.h"
#include "Characters/Mob/Component/MobAttackComponent.h"

#include "BossAttackComponent.generated.h"

class AActor;
class UAnimMontage;

class UMobAttackComponent;
class ABossBase;

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
	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RoarMontage;

private:
	AActor* PlayerActor;
	bool bIsTimeDropRock;

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
	UPROPERTY(EditDefaultsOnly, Category = "Skill | DropRock", meta = (AllowPrivateAccess = "true"))
	float DropRockDuration = 5.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill | DropRock", meta = (AllowPrivateAccess = "true"))
	float DropRockInterval = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill | DropRock", meta = (AllowPrivateAccess = "true"))
	float DropRockFirstSpawnDelay = 1.0f;


	FTimerHandle DropRockTickTimerHandle;
	FTimerHandle DropRockDurationTimerHandle;

	bool bIsDroppingRocks = false;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "BossSkill")
	void DropRockSkill();
	void DropRockSkill_Implementation();

	void SpawnRock(const FVector& Location);
	void DropRockTick();
	void EndDropRockSkill();

	UFUNCTION()
	virtual void OnDropRockAnimationEnd(UAnimMontage* Montage, bool bInterrupted);

private:
	FVector GetSpawnLocationInDropRockSkill();

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
