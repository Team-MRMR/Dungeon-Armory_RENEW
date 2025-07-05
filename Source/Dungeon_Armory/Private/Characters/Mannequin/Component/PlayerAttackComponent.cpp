// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mannequin/Component/PlayerAttackComponent.h"

#include "Characters/Core/Component/CharacterStatComponent.h"
#include "Characters/Core/Interface/IDamageable.h"

#include "Characters/Mannequin/Manny.h"

#include "GatherableActor/GatherableActorBase.h"

#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"

UPlayerAttackComponent::UPlayerAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ComboAttackSections = { "Combo1", "Combo2", "Combo3" };

	CurrentComboIndex = 2;
	bIsMontageEnded = true;
	bNextCombo = true;
	bCanReceiveInput = false;
}

void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayerCharacter = Cast<AManny>(GetOwner());
	if (OwnerPlayerCharacter)
	{
		AnimInstance = OwnerPlayerCharacter->GetMesh()->GetAnimInstance();
		Stat = OwnerPlayerCharacter->FindComponentByClass<UCharacterStatComponent>();
	}
}

void UPlayerAttackComponent::StartAttack()
{
	const float ConsumptionStamina = Stat->Stamina.AttackConsumption;
	const float CurrentStamina = Stat->Stamina.GetCurrent();

	// 현재 스태미너가 소비 스태미너보다 작으면 공격할 수 없음
	if (CurrentStamina <= ConsumptionStamina)
		return;

	if (bCanReceiveInput)
	{
		bNextCombo = true;
	}

	if (bIsMontageEnded)
	{
		ProceedCombo();
	}
}

void UPlayerAttackComponent::ProceedCombo()
{
	if (!ComboAttackMontage || !AnimInstance)
		return;

	if (AnimInstance->Montage_IsPlaying(ComboAttackMontage))
		return;

	PlayComboAttackMontage(CurrentComboIndex);

	bNextCombo = false;		// 콤보 소비
	CurrentComboIndex++;	// 다음 콤보 인덱스
}

void UPlayerAttackComponent::PlayComboAttackMontage(int32 ComboIndex)
{
	if (!ComboAttackMontage || !AnimInstance)
		return;

	if (!ComboAttackSections.IsValidIndex(ComboIndex))
		return;

	AnimInstance->Montage_Play(ComboAttackMontage);
	AnimInstance->Montage_JumpToSection(ComboAttackSections[ComboIndex], ComboAttackMontage);

	bIsMontageEnded = false;
}

float UPlayerAttackComponent::CalculateDamage(UCharacterStatComponent* Attacker, UCharacterStatComponent* Defender)
{
	return Super::CalculateDamage(Attacker, Defender);
}

// AttackEndNotify에서 호출
void UPlayerAttackComponent::OnAttackEnd()
{
	if (bNextCombo == false || CurrentComboIndex == ComboAttackSections.Num())
	{
		// 1. 다음 콤보 입력이 없거나
		// 2. 콤보를 모두 수행했다면
		CurrentComboIndex = 0;	// 콤보 몽타주 인덱스 초기화
	}

	bIsMontageEnded = true;		// 애니메이션 몽타주 종료

	bCanReceiveInput = false;	// 따라서 콤보 입력 불가

	// 다음 콤보 입력이 있었다면 바로 다음 콤보로 진행
	// 콤보 이어서 실행
	if (bNextCombo)
	{
		ProceedCombo();
	}
}

// ReceiveInputNotify에서 호출
void UPlayerAttackComponent::ReceiveInput()
{
	bCanReceiveInput = true;
}

// AttackNotify
void UPlayerAttackComponent::OnAttack()
{
	if (!GetOwner())
		return;

	FVector Start, End;
	FRotator ViewRot;
	GetOwner()->GetActorEyesViewPoint(Start, ViewRot);
	End = Start + ViewRot.Vector() * Stat->AttackableDistance;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FRotationMatrix::MakeFromZ(End - Start).ToQuat(),
		ECC_Pawn,
		FCollisionShape::MakeCapsule(
			Stat->AttackRadius,
			Stat->AttackRange * 0.5f
		),
		Params
	);

#if WITH_EDITOR
	DrawDebugCapsule(
		GetWorld(),
		(Start + End) * 0.5f,
		Stat->AttackRange * 0.5f,
		Stat->AttackRadius,
		FRotationMatrix::MakeFromZ(End - Start).ToQuat(),
		bHit ? FColor::Red : FColor::Green,
		false,
		0.5f
	);
#endif

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			DrawDebugSphere(
				GetWorld(),
				Hit.ImpactPoint,
				5.f,
				12,
				FColor::Red,
				false,
				0.5f
			);

			AActor* HitActor = Hit.GetActor();
			if (!HitActor)
				continue;

			auto TargetStat = HitActor->FindComponentByClass<UCharacterStatComponent>();
			if (TargetStat)
			{
				IIDamageable* DamagedActor = Cast<IIDamageable>(HitActor);
				if (DamagedActor && Stat)
				{
					const float DamageAmount = CalculateDamage(Stat, TargetStat);
					DamagedActor->ReceiveDamage(DamageAmount);

					OwnerPlayerCharacter->DecreaseDurability();  // 도구 내구도 감소

					const float ConsumptionStamina = Stat->Stamina.AttackConsumption;
					Stat->ConsumeStamina(ConsumptionStamina); // 스태미너 소비

					continue;
				}
			}
		}
	}
}
