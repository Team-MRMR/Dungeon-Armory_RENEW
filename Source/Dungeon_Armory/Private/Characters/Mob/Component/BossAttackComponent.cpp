// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mob/Component/BossAttackComponent.h"

UBossAttackComponent::UBossAttackComponent()
{
	BossOwner = Cast<ABossBase>(MobOwner);
	AttackCount = 0;
}

void UBossAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBossAttackComponent::StartAttack()
{
	Super::StartAttack();

	IncrementAttackCount();
	if (GetAttackCount() == SkillCycleByCount)
	{
		ResetAttackCount();

		DropRockSkill_Implementation();
	}
}

void UBossAttackComponent::DropRockSkill_Implementation()
{
	for (int count = 0; count < DropRockNumber; ++count)
	{
		FVector randomLocation = GetRandomPointInRadius();
		SpawnRockIndicator(randomLocation);
	}
}

FVector UBossAttackComponent::GetRandomPointInRadius()
{
	FVector DropCenter = BossOwner->GetActorLocation();
	float Radius = DropRockRadius;

	float RandX = FMath::RandRange(-Radius, Radius);
	float RandY = FMath::RandRange(-Radius, Radius);

	FVector TargetLocation = DropCenter + FVector(RandX, RandY, DropCenter.Z * DropRockHeight);

	return TargetLocation;
}

void UBossAttackComponent::SpawnRockIndicator(const FVector& Location)
{
	AActor* RockIndicatorInstance = BossOwner->GetWorld()->SpawnActor<AActor>(RockIndicatorClass, Location, FRotator::ZeroRotator);
	//RockIndicatorInstance->InitialLifeSpan = RockIndicatorDuration;
}