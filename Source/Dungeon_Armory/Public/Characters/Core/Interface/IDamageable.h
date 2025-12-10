// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UIDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUNGEON_ARMORY_API IIDamageable
{
	GENERATED_BODY()

public:
	void CreateAttackComponent();

	UFUNCTION(BlueprintNativeEvent, Category = "Damage")
	void ReceiveDamage(float DamageAmount);

	UFUNCTION(BlueprintNativeEvent, Category = "Damage")
	void Die();
};
