// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mob/Mob.h"

AMob::AMob()
{
	CreateAttackComponent(nullptr);
}

void AMob::BeginPlay()
{
	Super::BeginPlay();
}

void AMob::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);
}

void AMob::CreateAttackComponent(UAttackComponentBase* NewAttackComponent)
{
	if (AttackComponent != nullptr)
	{
		return;
	}

	if (NewAttackComponent == nullptr)
	{
		auto MobAttackComponent = CreateDefaultSubobject<UMobAttackComponent>(TEXT("MobAttackComponent"));
		auto AttackComponentBase = Cast<UAttackComponentBase>(MobAttackComponent);
		Super::CreateAttackComponent(AttackComponentBase);
	}
}


//void AMob::CreateAttackComponent(UAttackComponentBase* NewAttackComponent)
//{
//	if (AttackComponent != nullptr)
//	{
//		return;
//	}
//
//	auto MobAttackComponent = CreateDefaultSubobject<UMobAttackComponent>(TEXT("MobAttackComponent"));
//	auto AttackComponentBase = Cast<UAttackComponentBase>(MobAttackComponent);
//	Super::CreateAttackComponent(AttackComponentBase);
//}
