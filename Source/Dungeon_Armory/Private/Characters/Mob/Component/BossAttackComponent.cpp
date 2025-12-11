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

		DropRockSkill();
	}
}