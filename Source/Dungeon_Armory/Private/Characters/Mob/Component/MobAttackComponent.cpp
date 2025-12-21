// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Mob/Component/MobAttackComponent.h"
#include "Characters/Mob/MobBase.h"

#include "Characters/Core/Component/CharacterStatComponent.h"
#include "Characters/Core/Animation/AttackNotify.h"
#include "Characters/Core/Animation/AttackEndNotify.h"


// Sets default values for this component's properties
UMobAttackComponent::UMobAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MobOwner = Cast<AMobBase>(GetOwner());

	bIsStartedAttack = false;
	bIsEndedAttack = false;
	bCanAttack = true;
}

// Called when the game starts
void UMobAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = MobOwner->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 델리게이트 등록
		AnimInstance->OnMontageEnded.AddDynamic(this, &UMobAttackComponent::OnAttackAnimationEnd);
	}
}

// Called every frame
void UMobAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsStartedAttack)
	{
		ElapsedTime += DeltaTime;
		if (bIsEndedAttack && StatComponent->GetAttackCooldown() <= ElapsedTime)
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
	bIsCritical = (Chance <= StatComponent->CriticalChance) ? true : false;

	if (bIsCritical)
	{
		const float playRate = StatComponent->GetAttackPlayRate(CriticalAttackMontage->GetPlayLength());
		
		AnimInstance->Montage_Play(
			CriticalAttackMontage,
			playRate,
			EMontagePlayReturnType::MontageLength,
			0.0f,
			true
		);	
	}
	else
	{
		const float playRate = StatComponent->GetAttackPlayRate(NormalAttackMontage->GetPlayLength());

		AnimInstance->Montage_Play(
			NormalAttackMontage,
			playRate,
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
	const float TraceDistance = StatComponent->AttackableDistance;
	const FVector End = Start + Forward * TraceDistance;

	const float Radius = StatComponent->AttackRadius;
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,	// PlayerTraceChannel
		FCollisionShape::MakeSphere(Radius),
		Params
	);

	FColor TraceColor = bHit ? FColor::Red : FColor::Green;

//#if WITH_EDITOR
//	DrawDebugCapsule(
//		GetWorld(),
//		(Start + End) * 0.5f,
//		TraceDistance * 0.5f,
//		Radius,
//		FRotationMatrix::MakeFromZ(End - Start).ToQuat(),
//		TraceColor,
//		false,
//		0.25f
//	);
//#endif

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
//#if WITH_EDITOR
//			DrawDebugSphere(
//				GetWorld(),
//				Hit.ImpactPoint,
//				1.0f,
//				12,
//				FColor::Red,
//				false,
//				0.25f
//			);
//#endif

			AActor* HitActor = Hit.GetActor();
			if (!HitActor)
				return;

			auto TargetStat = HitActor->FindComponentByClass<UCharacterStatComponent>();
			if (!TargetStat)
				return;

			IIDamageable* DamagedActor = Cast<IIDamageable>(HitActor);
			if (DamagedActor && StatComponent)
			{
				const float DamageAmount = CalculateDamage(StatComponent, TargetStat);
				DamagedActor->Execute_ReceiveDamage(HitActor, DamageAmount);
			}
		}
	}
}

void UMobAttackComponent::OnAttackEnd()
{
	bIsEndedAttack = true;
}

void UMobAttackComponent::OnAttackAnimationEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bIsEndedAttack = true;
}

float UMobAttackComponent::CalculateDamage(UCharacterStatComponent* Attacker, UCharacterStatComponent* Defender)
{
	return Super::CalculateDamage(Attacker, Defender);
}

