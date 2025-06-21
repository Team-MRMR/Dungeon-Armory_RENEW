// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Core/Component/AttackComponentBase.h"
#include "Characters/Core/Component/CharacterStatComponent.h"

#include "Characters/Core/Interface/IDamageable.h"

#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"

UAttackComponentBase::UAttackComponentBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttackComponentBase::BeginPlay()
{
	Super::BeginPlay();

	auto OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		Stat = OwnerCharacter->FindComponentByClass<UCharacterStatComponent>();
	}
}

void UAttackComponentBase::StartAttack()
{
	
}

// AttackNotify
void UAttackComponentBase::OnAttack()
{

}

// AttackEndNotify
void UAttackComponentBase::OnAttackEnd()
{

}

float UAttackComponentBase::CalculateDamage(UCharacterStatComponent* Attacker, UCharacterStatComponent* Defender)
{
	int8 AttackerElement = static_cast<int8>(Attacker->WeaponElementType);
	int8 DefenderElement = static_cast<int8>(Defender->WeaponElementType);
	float ElementalFactor = ElementalCompatibility[AttackerElement][DefenderElement];

	float Damage = (Attacker->BaseAttackDamage * ElementalFactor) - Defender->Defense;

	return Damage;
}
