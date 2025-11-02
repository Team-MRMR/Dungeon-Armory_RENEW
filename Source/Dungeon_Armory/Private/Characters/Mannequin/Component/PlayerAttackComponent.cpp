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

	CurrentComboIndex = -1;
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

	// ProceedCombo에서 델리게이트 등록
}

/*
* Attack - public
*/

void UPlayerAttackComponent::StartAttack()
{
	// 도구 타입 업데이트
	UpdateToolType();
	if (ToolType != EToolType::Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("ToolType is not Weapon. Cannot attack."));
		return;
	}

	// 애니메이션 인스턴스 최신화
	UAnimInstance* LatestAnimInstance = OwnerPlayerCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance != LatestAnimInstance)
	{
		AnimInstance = LatestAnimInstance;
		AnimInstance->OnMontageEnded.AddDynamic(this, &UPlayerAttackComponent::OnAttackAnimationEnd);

		// 초기화 작업
		CurrentComboIndex = 2;
		bIsMontageEnded = true;
		bNextCombo = true;
		bCanReceiveInput = false;
	}

	// 스태미너 소비 확인
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
		ECC_GameTraceChannel2,
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
	if (bHit && Stat)
	{
		// 공격 범위 내의 모든 액터에 대해 처리
		for (const FHitResult& Hit : HitResults)
		{
#if WITH_EDITOR
			DrawDebugSphere(
				GetWorld(),
				Hit.ImpactPoint,
				5.f,
				12,
				FColor::Red,
				false,
				0.5f
			);
#endif
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
					DamagedActor->Execute_ReceiveDamage(HitActor, DamageAmount);

					continue;
				}
			}
		}
	
		OwnerPlayerCharacter->Execute_DecreaseDurability(OwnerPlayerCharacter);  // 도구 내구도 감소

		const float ConsumptionStamina = Stat->Stamina.AttackConsumption;
		Stat->ConsumeStamina(ConsumptionStamina); // 스태미너 소비
	}
}

void UPlayerAttackComponent::OnAttackEnd()
{
	if (bNextCombo == false || ComboAttackSections.Num() <= CurrentComboIndex)
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


/*
* Attack - protected
*/

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

void UPlayerAttackComponent::ResetCooldown()
{
	bIsCooldownTime = false;
}

/*
* Attack - private
*/

void UPlayerAttackComponent::UpdateToolType()
{
	auto OwnerActor = GetOwner();
	if (!OwnerActor)
		return;

	auto IToolEuipable = Cast<IIToolEuipable>(OwnerActor);
	if (!IToolEuipable)
		return;

	ToolType = IToolEuipable->Execute_GetToolType(OwnerActor);
}

float UPlayerAttackComponent::CalculateDamage(UCharacterStatComponent* Attacker, UCharacterStatComponent* Defender)
{
	return Super::CalculateDamage(Attacker, Defender);
}

/*
* Animation - private
*/

void UPlayerAttackComponent::OnAttackAnimationEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (ComboAttackSections.Num() <= CurrentComboIndex)
	{
		// 콤보를 모두 수행했다면
		CurrentComboIndex = 0;	// 콤보 몽타주 인덱스 초기화
	}

	bIsMontageEnded = true;		// 애니메이션 몽타주 종료
	bCanReceiveInput = false;	// 콤보 입력 불가

}

void UPlayerAttackComponent::PlayComboAttackMontage(int32 ComboIndex)
{
	if (!ComboAttackMontage || !AnimInstance)
		return;

	if (!ComboAttackSections.IsValidIndex(ComboIndex))
		return;

	animLength = ComboAttackMontage->GetSectionLength(ComboIndex);
	animPlayRate = Stat->GetAttackPlayRate(animLength);

	AnimInstance->Montage_Play(
		ComboAttackMontage,
		Stat->GetAttackPlayRate(animLength),
		EMontagePlayReturnType::Duration,
		0.0f,
		true
	);
	AnimInstance->Montage_JumpToSection(ComboAttackSections[ComboIndex], ComboAttackMontage);

	bIsMontageEnded = false;

	// 공격과 공격 간의 시간 제어 설정
	{	
		// 몽타주 재생 후, 쿨다운 타임 설정 및 시작
		attackCooldownTime = Stat->GetAttackCooldown();
		bIsCooldownTime = true;

		// 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(
			CooldownTimerHandle,
			this,
			&UPlayerAttackComponent::ResetCooldown,
			attackCooldownTime,
			false
		);
	}
}