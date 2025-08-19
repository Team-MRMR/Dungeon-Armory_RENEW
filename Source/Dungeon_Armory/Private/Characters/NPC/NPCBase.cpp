// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/NPCBase.h"

#include "Characters/NPC/AI/LocationPoint.h"
#include "Characters/NPC/AI/NPCAIController.h"

#include "Characters/Core/Component/CharacterStatComponent.h"
#include "Characters/Core/AI/Team/TeamComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

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

void ANPCBase::SetCurrentState(NPCState NewState)
{
	CurrentState = NewState;

	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
		return;

	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if (!BlackboardComponent)
		return;

	switch (CurrentState)
	{
	case NPCState::Roaming:
		SetIsShopping(false);
		break;
	case NPCState::Shopping:
		SetIsShopping(true);
		break;
	}

	BlackboardComponent->SetValueAsEnum(BBKeys::NPC::NPCState, static_cast<int8>(CurrentState));
}

void ANPCBase::SetIsShopping(bool _bIsShopping)
{
	bIsShopping = _bIsShopping;

	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
		return;

	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if (!BlackboardComponent)
		return;

	BlackboardComponent->SetValueAsBool(BBKeys::NPC::IsShopping, bIsShopping);
}

FVector const ANPCBase::GetPointLocation()
{
	if (RoammingPoints.Num() < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("RoammingPoints is empty!"));
		return GetActorLocation();
	}

	if (ShoppingPoints.Num() < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShoppingPoints is empty!"));
		return GetActorLocation();
	}

	ALocationPoint* LocationPoint;

	if (bIsShopping)
	{
		int32 RandomIndex = FMath::RandRange(0, ShoppingPoints.Num() - 1);
		LocationPoint = ShoppingPoints.IsValidIndex(RandomIndex) ? ShoppingPoints[RandomIndex] : nullptr;
		return LocationPoint->GetLocation();
	}
	else
	{
		LocationPoint = RoammingPoints.IsValidIndex(CurrentIndex) ? RoammingPoints[CurrentIndex] : nullptr;
		CurrentIndex = (++CurrentIndex) % RoammingPoints.Num();
		return LocationPoint->GetLocation();
	}
}