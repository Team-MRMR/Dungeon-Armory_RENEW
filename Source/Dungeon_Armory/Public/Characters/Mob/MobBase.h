// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Characters/Core/Interface/IDamageable.h"

//#include "GenericTeamAgentInterface.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "MobBase.generated.h"

class UAttackComponentBase;

class UCharacterStatComponent;
class UMovementControllerComponent;
class UBlackboardComponent;

class AMobAIController;

class USoundBase;

UCLASS(Abstract)
class DUNGEON_ARMORY_API AMobBase : public ACharacter, public IGenericTeamAgentInterface, public IIDamageable
{
	GENERATED_BODY() 

/***** Unreal *****/
public:
	AMobBase();

protected:
	virtual void BeginPlay() override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

/***** Mob *****/
protected:
	AMobAIController* MobAIController;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Mob", meta = (AllowPrivateAccess = "true"))
	float DisappearTime;

	UPROPERTY(EditDefaultsOnly, Category = "IDamageable", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DieMontage;

/***** Stat *****/
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCharacterStatComponent* StatComponent;

/***** Team *****/
public:
	virtual FGenericTeamId GetGenericTeamId() const override;

/***** Attack *****/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UAttackComponentBase* _AttackComponent;

/***** Sounds *****/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* MoveSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* AttackSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* DamagedSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* DieSound;

/***** IIDamageable *****/
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Die")
	void ReceiveDamage(float DamageAmount);
	virtual void ReceiveDamage_Implementation(float DamageAmount) override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Die")
	void Die();
	virtual void Die_Implementation() override;
};
