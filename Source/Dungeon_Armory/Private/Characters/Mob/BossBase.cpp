// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Mob/BossBase.h"

// Sets default values
ABossBase::ABossBase()
{
	CreateAttackComponent();
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

void ABossBase::CreateAttackComponent()
{
	if (AttackComponent != nullptr)
	{
		return;
	}

	auto BossAttackComponent = CreateDefaultSubobject<UBossAttackComponent>(TEXT("BossAttackComponent"));
	auto AttackComponentBase = Cast<UAttackComponentBase>(BossAttackComponent);
	Super::CreateAttackComponent(AttackComponentBase);
}
