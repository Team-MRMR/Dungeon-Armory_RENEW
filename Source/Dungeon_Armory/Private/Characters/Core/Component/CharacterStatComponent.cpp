// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Core/Component/CharacterStatComponent.h"

#include "Characters/Mob/MobBase.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	if (AMobBase* Mob = Cast<AMobBase>(GetOwner()))
	{
		OwnerCharacter = Mob;
	}
}

// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	auto World = GetWorld();
	if (World)
	{
		Stamina.Initialize(World);
	}

	SetSpeed(BaseSpeed);
	CurrentHealth = MaxHealth;

}
void UCharacterStatComponent::ApplySpeedModifier(float SpeedMultiplier, float Duration)
{
	//float NewSpeed = BaseSpeed * SpeedMultiplier;
	//SetMovementSpeed(NewSpeed);

	//// 일정 시간이 지나면 원래 속도로 복귀
	//GetWorldTimerManager().SetTimer(SpeedResetTimer, this, &ANPCCharacter::ResetSpeed, Duration, false);
}

void UCharacterStatComponent::ResetSpeed()
{
	SetSpeed(BaseSpeed);
}

void UCharacterStatComponent::SetSpeed(float NewSpeed)
{
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
}

float UCharacterStatComponent::GetSpeedForState(EMobState State) const
{
	switch (State)
	{
	case EMobState::Idle:
		return BaseSpeed * IdleSpeedFactor;

	case EMobState::Patrol:
		return BaseSpeed * PatrolSpeedFactor;

	case EMobState::Chase:
	case EMobState::Battle:
		return BaseSpeed * ChaseSpeedFactor;

	case EMobState::Dead:
		return BaseSpeed * DeadSpeedFactor;

	default:
		return BaseSpeed;
	}
}

void UCharacterStatComponent::UpdateStamina()
{
	Stamina.ApplyRegen();
}

void UCharacterStatComponent::ConsumeStamina(const float ConsumptionStamina)
{
	Stamina.Consume(ConsumptionStamina);
}

void UCharacterStatComponent::SetSpeedForState(EMobState State)
{
	float ChangedSpeed = GetSpeedForState(State);
	SetSpeed(ChangedSpeed);
}

void UCharacterStatComponent::ApplyDamage(const float DamageAmount)
{
	CurrentHealth -= DamageAmount;
}

float UCharacterStatComponent::GetAttackPlayRate(float AnimationLength) const
{
	const float SafeBaseAttackSpeed = FMath::Max(BaseAttackSpeed, 0.01f);

	// 1. 공격 속도가 느릴 때
	if (SafeBaseAttackSpeed < 1.0f)
	{
		return SafeBaseAttackSpeed;
	}
	// 2. 공격 속도가 빠르거나 같을 때
	else
	{
		float Cooldown = 1.0f / SafeBaseAttackSpeed;

		// 애니메이션 재생 시간이 Cooldown보다 짧다면 (쿨타임이 충분하면)
		if (AnimationLength <= Cooldown)
		{
			return 1.0f;
		}
		// 애니메이션 재생 시간이 Cooldown보다 길다면
		else
		{
			// PlayRate = AnimationLength * SafeBaseAttackSpeed = AnimationLength / Cooldown
			return AnimationLength * SafeBaseAttackSpeed;
		}

	}
}

float UCharacterStatComponent::GetLoggingPlayRate(float AnimationLength) const
{
	const float SafeLoggingSpeed = FMath::Max(LoggingSpeed, 0.01f);

	if (SafeLoggingSpeed < 1.0f)
	{
		return SafeLoggingSpeed;
	}
	else
	{
		float Cooldown = 1.0f / SafeLoggingSpeed;

		if (AnimationLength <= Cooldown)
		{
			return 1.0f;
		}
		else
		{
			return AnimationLength * SafeLoggingSpeed;
		}
	}
}

float UCharacterStatComponent::GetMiningPlayRate(float AnimationLength) const
{
	const float SafeMiningSpeed = FMath::Max(MiningSpeed, 0.01f);

	if (SafeMiningSpeed < 1.0f)
	{
		return SafeMiningSpeed;
	}
	else
	{
		float Cooldown = 1.0f / SafeMiningSpeed;

		if (AnimationLength <= Cooldown)
		{
			return 1.0f;
		}
		else
		{
			return AnimationLength * SafeMiningSpeed;
		}
	}
}
