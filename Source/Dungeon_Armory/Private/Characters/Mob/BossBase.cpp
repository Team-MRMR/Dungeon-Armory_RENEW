// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Mob/BossBase.h"
#include "Characters/Mob/Component/BossAttackComponent.h"

// Sets default values
ABossBase::ABossBase()
{
	CreateAttackComponent(nullptr);
}

// Called when the game starts or when spawned
void ABossBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABossBase::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);
}

void ABossBase::CreateAttackComponent(UAttackComponentBase* const NewAttackComponent)
{
	if (AttackComponent != nullptr)
	{
		return;
	}

	if (NewAttackComponent == nullptr)
	{
		auto BossAttackComponent = CreateDefaultSubobject<UBossAttackComponent>(TEXT("BossAttackComponent"));
		auto AttackComponentBase = Cast<UAttackComponentBase>(BossAttackComponent);
		Super::CreateAttackComponent(AttackComponentBase);
	}
}