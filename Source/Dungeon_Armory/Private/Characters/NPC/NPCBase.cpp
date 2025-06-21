// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/NPCBase.h"
#include "Characters/NPC/AI/LocationPoint.h"

#include "Characters/Core/Component/CharacterStatComponent.h"
#include "Characters/Core/AI/Team/TeamComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

/*
* Functions (Unreal)
*/

ANPCBase::ANPCBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// 스탯 컴포넌트 생성
	StatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("StatComponent"));

	// 팀 컴포넌트 생성
	TeamComponent = CreateDefaultSubobject<UTeamComponent>(TEXT("TeamComponent"));
}

// Called when the game starts or when spawned
void ANPCBase::BeginPlay()
{
	Super::BeginPlay();

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->MaxWalkSpeed = StatComponent->BaseSpeed;
	//GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	TeamComponent->SetTeamType(ETeamType::Mob);
}

void ANPCBase::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	//OutLocation = GetMesh()->GetSocketLocation(FName("EyeSocket"));	// 머리 위치를 기준으로 시점 설정
	//OutRotation = GetMesh()->GetSocketRotation(FName("EyeSocket"));	// 머리 위치를 기준으로 시점 설정

	OutLocation = GetActorLocation();
	OutRotation = GetActorRotation();
}

/*
* Functions (Team)
*/

FGenericTeamId ANPCBase::GetGenericTeamId() const
{
	return TeamComponent->GetGenericTeamId();
}

void ANPCBase::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamComponent->SetTeamType(static_cast<ETeamType>(NewTeamID.GetId()));
}

/*
* Functions (Behavior Tree)
*/

FVector ANPCBase::GetNextPoint()
{
	if (LocationPoints.Num() == 0)
	{
		return GetActorLocation();
	}

	const FVector NextPoint = LocationPoints[CurrLocationPointIndex]->GetPointLocation();
	CurrLocationPointIndex = (CurrLocationPointIndex + 1) % LocationPoints.Num();
	return NextPoint;
}

FVector ANPCBase::GetHomePoint() const
{
	if (!HomePoint)
	{
		return GetActorLocation();
	}

	return HomePoint->GetPointLocation();
}
