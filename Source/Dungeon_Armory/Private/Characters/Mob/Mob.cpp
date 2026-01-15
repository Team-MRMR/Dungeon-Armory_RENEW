// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Mob/Mob.h"
#include "Characters/Mob/Component/MobAttackComponent.h"

AMob::AMob()
{
	_AttackComponent = CreateDefaultSubobject<UMobAttackComponent>(TEXT("AttackComponent"));
}

void AMob::BeginPlay()
{
	Super::BeginPlay();
}

void AMob::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);
}