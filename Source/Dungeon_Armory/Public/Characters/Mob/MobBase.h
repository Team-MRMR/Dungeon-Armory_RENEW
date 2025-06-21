// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Characters/Core/Interface/IDamageable.h"

#include "GenericTeamAgentInterface.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "MobBase.generated.h"

class UCharacterStatComponent;
class UMobAttackComponent;
class UMovementControllerComponent;
class UTeamComponent;
class UBlackboardComponent;

class AAIController;

class USoundBase;

UCLASS()
class DUNGEON_ARMORY_API AMobBase : public ACharacter, public IGenericTeamAgentInterface, public IIDamageable
{
	GENERATED_BODY()

/***** Unreal *****/
public:
	AMobBase();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

/***** Stat *****/
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCharacterStatComponent* StatComponent;

/***** Team *****/
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UTeamComponent* TeamComponent;

/***** Attack *****/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UMobAttackComponent* AttackComponent;

/***** Movement *****/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UMovementControllerComponent* MovementControllerComponent;

protected:
	// 피격 및 사망 시 재생할 사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* DieSound;

public:
	void ReceiveDamage(float DamageAmount) override;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Die")
	void Die();
	virtual void Die_Implementation() { }
};
