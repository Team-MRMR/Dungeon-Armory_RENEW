// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Mob/Component/MobAttackComponent.h"
#include "Characters/Mob/MobBase.h"

#include "Characters/Core/Component/CharacterStatComponent.h"
#include "Characters/Core/Animation/AttackNotify.h"
#include "Characters/Core/Animation/AttackEndNotify.h"


// Sets default values for this component's properties
UMobAttackComponent::UMobAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsStartedAttack = false;
	bIsEndedAttack = false;
	bCanAttack = true;
}

// Called when the game starts
void UMobAttackComponent::BeginPlay()
{
	// �ִ� �ν��Ͻ� ����
	// ���� ������Ʈ ����
	Super::BeginPlay();
}

// Called every frame
void UMobAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsStartedAttack)
	{
		ElapsedTime += DeltaTime;
		if (bIsEndedAttack && ElapsedTime >= Stat->GetAttackCooldown())
		{
			bIsStartedAttack = false;
			bCanAttack = true;
		}
	}
}

void UMobAttackComponent::StartAttack()
{
	if (!AnimInstance)
		return;

	if (!NormalAttackMontage || !CriticalAttackMontage)
		return;

	float Chance = FMath::FRandRange(0.0f, 1.0f);
	bIsCritical = (Chance <= Stat->CriticalChance) ? true : false;

	if (bIsCritical)
	{
		const float AttackRate = Stat->GetAttackPlayRate(CriticalAttackMontage->GetPlayLength());
		
		// ũ��Ƽ�� ���� ���
		AnimInstance->Montage_Play(
			CriticalAttackMontage,
			AttackRate,
			EMontagePlayReturnType::MontageLength,
			0.0f,
			true
		);	
	}
	else
	{
		const float AttackRate = Stat->GetAttackPlayRate(NormalAttackMontage->GetPlayLength());

		// �Ϲ� ���� ���
		AnimInstance->Montage_Play(
			NormalAttackMontage,
			AttackRate,
			EMontagePlayReturnType::MontageLength,
			0.0f,
			true
		);
	}
	

	ElapsedTime = 0.0f;
	bIsStartedAttack = true;
	bIsEndedAttack = false;

	bCanAttack = false;
}

bool UMobAttackComponent::GetCanAttack() const
{
	return bCanAttack;
}

void UMobAttackComponent::OnAttack()
{
	if (!GetOwner())
		return;

	const FVector Start = GetOwner()->GetActorLocation();
	const FVector Forward = GetOwner()->GetActorForwardVector();
	const float TraceDistance = Stat->AttackableDistance;
	const FVector End = Start + Forward * TraceDistance;

	const float Radius = Stat->AttackRadius;
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(Radius),
		Params
	);

	FColor TraceColor = bHit ? FColor::Red : FColor::Green;

	DrawDebugCapsule(
		GetWorld(),
		(Start + End) * 0.5f,
		TraceDistance * 0.5f,
		Radius,
		FRotationMatrix::MakeFromZ(End - Start).ToQuat(),
		TraceColor,
		false,
		0.25f
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			DrawDebugSphere(
				GetWorld(),
				Hit.ImpactPoint,
				1.0f,
				12,
				FColor::Red,
				false,
				0.25f
			);

			AActor* HitActor = Hit.GetActor();
			if (!HitActor)
				return;

			auto TargetStat = HitActor->FindComponentByClass<UCharacterStatComponent>();
			if (!TargetStat)
				return;

			IIDamageable* DamagedActor = Cast<IIDamageable>(HitActor);
			if (DamagedActor && Stat)
			{
				const float DamageAmount = CalculateDamage(Stat, TargetStat);
				DamagedActor->ReceiveDamage(DamageAmount);
			}
		}
	}
}

void UMobAttackComponent::OnAttackEnd()
{
	bIsEndedAttack = true;
}

float UMobAttackComponent::CalculateDamage(UCharacterStatComponent* Attacker, UCharacterStatComponent* Defender)
{
	return Super::CalculateDamage(Attacker, Defender);
}

