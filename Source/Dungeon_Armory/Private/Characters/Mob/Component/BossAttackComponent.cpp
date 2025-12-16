// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Mob/Component/BossAttackComponent.h"

#include "Characters/Core/AI/AIControllerBase.h"

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

	bIsDroppingRocks = true;

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

	// 1초마다 돌 스폰
	GetWorld()->GetTimerManager().SetTimer(
		DropRockTickTimerHandle,
		this,
		&UBossAttackComponent::DropRockTick,
		DropRockInterval,
		true
	);

	// 5초 후 스킬 종료
	GetWorld()->GetTimerManager().SetTimer(
		DropRockDurationTimerHandle,
		this,
		&UBossAttackComponent::EndDropRockSkill,
		DropRockDuration,
		false
	);

	// DropRock 스폰
	for (int count = 0; count < DropRockNumber; ++count)
	{
		FVector randomLocation = GetRandomPointInDropRockSpawnRadius();
		SpawnRock(randomLocation);
	}
}

void UBossAttackComponent::SpawnRock(const FVector& Location)
{
	GetWorld()->SpawnActor<AActor>(RockClass, Location, FRotator::ZeroRotator);
}

void UBossAttackComponent::DropRockTick()
{
	if (!bIsDroppingRocks)
		return;

	FVector SpawnLocation = GetRandomPointInDropRockSpawnRadius();
	SpawnRock(SpawnLocation);
}

void UBossAttackComponent::EndDropRockSkill()
{
	bIsDroppingRocks = false;

	GetWorld()->GetTimerManager().ClearTimer(DropRockTickTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DropRockDurationTimerHandle);
}

void UBossAttackComponent::OnDropRockAnimationEnd(UAnimMontage* Montage, bool bInterrupted)
{
	// 애니메이션이 먼저 끝나도
	// 스킬은 타이머 기준으로 동작
	// bIsDroppingRocks = false;
}

FVector UBossAttackComponent::GetRandomPointInDropRockSpawnRadius()
{
	AActor* player = Cast<AAIControllerBase>(BossOwner->GetController())->GetDetectedPlayer();

	FVector dropCenter = player->GetActorLocation();
	float radius = DropRockSpawnRadius;

	float RandX = FMath::RandRange(-radius, radius);
	float RandY = FMath::RandRange(-radius, radius);

	FVector targetLocation = dropCenter + FVector(RandX, RandY, dropCenter.Z + DropRockSpawnHeight);

	return targetLocation;
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
	if (SpawnMobList.Num() == 0)
		return;

	// 랜덤으로 몹 선택
	int RandomIndex = FMath::RandRange(0, SpawnMobList.Num() - 1);
	auto SpawnedMob = BossOwner->GetWorld()->SpawnActor<AMob>(SpawnMobList[RandomIndex], Location, FRotator::ZeroRotator);
}

void UBossAttackComponent::OnSpawnMobAnimationEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bIsSpawningMobs = false;
}

FVector UBossAttackComponent::GetRandomPointInMobSpawnRadius()
{
	AActor* player = Cast<AAIControllerBase>(BossOwner->GetController())->GetDetectedPlayer();

	FVector dropCenter = player->GetActorLocation();
	float radius = MobSpawnRadius;

	float RandX = FMath::RandRange(-radius, radius);
	float RandY = FMath::RandRange(-radius, radius);

	FVector targetLocation = dropCenter + FVector(RandX, RandY, dropCenter.Z + MobSpawnHeight);

	return targetLocation;
}

#pragma endregion
