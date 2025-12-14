// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mob/Component/BossAttackComponent.h"

UBossAttackComponent::UBossAttackComponent()
{
	BossOwner = Cast<ABossBase>(MobOwner);
	AttackCount = 0;

	bIsTimeDropRock = true;
}

void UBossAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBossAttackComponent::StartAttack()
{
	if (bIsDroppingRocks || bIsSpawningMobs)
	{
		return;
	}

	if (GetAttackCount() == SkillCycleByCount)
	{
		ResetAttackCount();

		bIsTimeDropRock ? DropRockSkill_Implementation() : SpawnMobSkill_Implementation();
		bIsTimeDropRock = !bIsTimeDropRock;
		return;
	}

	Super::StartAttack();
	IncrementAttackCount();
}

#pragma region Boss Skill - DropRock

void UBossAttackComponent::DropRockSkill_Implementation()
{
	if (!AnimInstance)
		return;

	if (!RoarMontage)
		return;

	// 몽타주 재생
	AnimInstance->Montage_Play(RoarMontage);

	// 특정 몽타주 전용 종료 델리게이트 바인딩
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(
		this,
		&UBossAttackComponent::OnDropRockAnimationEnd
	);

	AnimInstance->Montage_SetEndDelegate(
		EndDelegate,
		RoarMontage
	);

	// DropRock 스폰
	for (int count = 0; count < DropRockNumber; ++count)
	{
		FVector randomLocation = GetRandomPointInDropRockSpawnRadius();
		SpawnRock(randomLocation);
	}

	bIsDroppingRocks = true;
}

void UBossAttackComponent::SpawnRock(const FVector& Location)
{
	BossOwner->GetWorld()->SpawnActor<AActor>(RockClass, Location, FRotator::ZeroRotator);
}

void UBossAttackComponent::OnDropRockAnimationEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bIsDroppingRocks = false;
}

FVector UBossAttackComponent::GetRandomPointInDropRockSpawnRadius()
{
	FVector DropCenter = BossOwner->GetActorLocation();
	float Radius = DropRockSpawnRadius;

	float RandX = FMath::RandRange(-Radius, Radius);
	float RandY = FMath::RandRange(-Radius, Radius);

	FVector TargetLocation = DropCenter + FVector(RandX, RandY, DropCenter.Z + DropRockSpawnHeight);

	return TargetLocation;
}

#pragma endregion


#pragma region Boss Skill - SpawnMob

void UBossAttackComponent::SpawnMobSkill_Implementation()
{
	if (!AnimInstance)
		return;

	if (!RoarMontage)
		return;

	// 몽타주 재생
	AnimInstance->Montage_Play(RoarMontage);

	// 특정 몽타주 전용 종료 델리게이트 바인딩
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(
		this,
		&UBossAttackComponent::OnSpawnMobAnimationEnd
	);

	AnimInstance->Montage_SetEndDelegate(
		EndDelegate,
		RoarMontage
	);

	// Mob 스폰
	for (int count = 0; count < SpawnMobNumber; ++count)
	{
		FVector randomLocation = GetRandomPointInMobSpawnRadius();
		SpawnMob(randomLocation);
	}

	bIsSpawningMobs = true;
}

void UBossAttackComponent::SpawnMob(const FVector& Location)
{
	// 랜덤으로 몹 선택
	int RandomIndex = FMath::RandRange(0, SpawnMobList.Num() - 1);
	BossOwner->GetWorld()->SpawnActor<AMob>(SpawnMobList[RandomIndex], Location, FRotator::ZeroRotator);
}

void UBossAttackComponent::OnSpawnMobAnimationEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bIsSpawningMobs = false;
}

FVector UBossAttackComponent::GetRandomPointInMobSpawnRadius()
{
	FVector DropCenter = BossOwner->GetActorLocation();
	float Radius = MobSpawnRadius;

	float RandX = FMath::RandRange(-Radius, Radius);
	float RandY = FMath::RandRange(-Radius, Radius);

	FVector TargetLocation = DropCenter + FVector(RandX, RandY, DropCenter.Z + MobSpawnHeight);

	return TargetLocation;
}

#pragma endregion
