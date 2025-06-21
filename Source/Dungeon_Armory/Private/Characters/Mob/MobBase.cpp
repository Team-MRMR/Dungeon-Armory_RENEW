// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mob/MobBase.h"
#include "Characters/Mob/Component/MobAttackComponent.h"

#include "Characters/Core/Component/CharacterStatComponent.h"
#include "Characters/Core/Component/MovementControllerComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/Core/AI/Team/TeamComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIController.h"

// sound
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMobBase::AMobBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// 스탯 컴포넌트 생성
	StatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("StatComponent"));

	// 공격 컴포넌트 생성
	AttackComponent = CreateDefaultSubobject<UMobAttackComponent>(TEXT("AttackComponent"));

	// 팀 컴포넌트 생성
	TeamComponent = CreateDefaultSubobject<UTeamComponent>(TEXT("TeamComponent"));

	// 이동 컨트롤러 컴포넌트 생성
	MovementControllerComponent = CreateDefaultSubobject<UMovementControllerComponent>(TEXT("MovementControllerComponent"));
}

// Called when the game starts or when spawned
void AMobBase::BeginPlay()
{
	Super::BeginPlay();

	// (Pawn) 컨트롤러 회전 제어 해제
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 컨트롤러 회전은 무시
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// 이동	방향으로 회전하도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 회전 속도 설정 (원하는 속도로 조정)
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = StatComponent->BaseSpeed;
	// GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	// GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	TeamComponent->SetTeamType(ETeamType::Mob);
}

void AMobBase::Tick(float DeltaSeconds)
{
}

void AMobBase::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	//OutLocation = GetMesh()->GetSocketLocation(FName("EyeSocket"));	// 머리 위치를 기준으로 시점 설정
	//OutRotation = GetMesh()->GetSocketRotation(FName("EyeSocket"));	// 머리 위치를 기준으로 시점 설정

	OutLocation = GetActorLocation();
	OutRotation = GetActorRotation();
}

void AMobBase::ReceiveDamage(float DamageAmount)
{
	if (StatComponent)
	{
		StatComponent->ApplyDamage(DamageAmount);

		if (0.0f <= StatComponent->CurrentHealth)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(this, DieSound, GetActorLocation());
			Die();	// 죽음 처리
		}
	}
}
