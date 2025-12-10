// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mob/Mob.h"

AMob::AMob()
{
	CreateAttackComponent();
}

void AMob::BeginPlay()
{
	Super::BeginPlay();
}

void AMob::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);
}

void AMob::CreateAttackComponent()
{
	if (AttackComponent != nullptr)
	{
		return;
	}

	auto MobAttackComponent = CreateDefaultSubobject<UMobAttackComponent>(TEXT("MobAttackComponent"));
	auto AttackComponentBase = Cast<UAttackComponentBase>(MobAttackComponent);
	Super::CreateAttackComponent(AttackComponentBase);
}
