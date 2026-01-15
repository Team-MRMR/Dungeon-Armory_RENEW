// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Mob/BossBase.h"
#include "Characters/Mob/Component/BossAttackComponent.h"

ABossBase::ABossBase()
{
	_AttackComponent = CreateDefaultSubobject<UBossAttackComponent>(TEXT("AttackComponent"));
}

void ABossBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABossBase::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);
}