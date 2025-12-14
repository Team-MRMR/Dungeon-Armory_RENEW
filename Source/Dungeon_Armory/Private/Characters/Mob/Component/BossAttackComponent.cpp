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
	if (bIsDroppingRocks == false)
	{
		IncrementAttackCount();
		if (GetAttackCount() == SkillCycleByCount)
		{
			ResetAttackCount();

			DropRockSkill_Implementation();
		}
		else
		{
			Super::StartAttack();
		}
	}
}

void UBossAttackComponent::DropRockSkill_Implementation()
{
	if (!AnimInstance)
		return;

	if (!RoarMontage)
	{
		return;
	}

	const float playRate = StatComponent->GetAttackPlayRate(RoarMontage->GetPlayLength());

	// 몽타주 재생
	AnimInstance->Montage_Play(
		RoarMontage,
		playRate,
		EMontagePlayReturnType::MontageLength,
		0.0f,
		true
	);

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
		FVector randomLocation = GetRandomPointInRadius();
		SpawnRock(randomLocation);
	}

	bIsDroppingRocks = true;
}

void UBossAttackComponent::SpawnRock(const FVector& Location)
{
	AActor* RockIndicatorInstance = BossOwner->GetWorld()->SpawnActor<AActor>(RockIndicatorClass, Location, FRotator::ZeroRotator);
	//RockIndicatorInstance->InitialLifeSpan = RockIndicatorDuration;
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

void UBossAttackComponent::OnDropRockAnimationEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bIsDroppingRocks = false;
}
