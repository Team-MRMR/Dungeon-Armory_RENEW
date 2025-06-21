// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Characters/Core/Stat/StaminaStat.h"

#include "CharacterStatComponent.generated.h"

UENUM(BlueprintType)
enum class EMobState : uint8
{
    Idle,
    Patrol,
    Chase,
    Battle,
    Dead
};

UENUM(BlueprintType)
enum class EElementalType : uint8
{
	None,   // 없음

    Aqua,   // 물
    Ignis,  // 불
	Terra   // 땅
};

constexpr float ElementalCompatibility[4][4] =
{
    // None, Aqua, Ignis, Terra
    {1.0f, 1.0f, 1.0f, 1.0f}, // None
    {1.0f, 1.0f, 1.2f, 0.8f}, // Aqua
    {1.0f, 0.8f, 1.0f, 1.2f}, // Ignis
    {1.0f, 1.2f, 0.8f, 1.0f}, // Terra
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEON_ARMORY_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

/***** Character *****/
private:
    ACharacter* OwnerCharacter;

// --- HP관련 스탯 ---
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | HP")
    float MaxHealth = 100.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat | HP")
    float CurrentHealth;

// --- Stamina 관련 스탯 ---
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Stamina")
    FStaminaStat Stamina;

// --- 플레이어 및 무기 속성 관련 스탯 ---
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Elemental")
	EElementalType WeaponElementType = EElementalType::None;

// --- 공격력 관련 스탯 ---
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Attack | Damage")
    float BaseAttackDamage = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Attack | Speed")
	float BaseAttackSpeed = 1.0f;   // 초당 공격 횟수

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Attack | Ciritical")
    float CriticalChance = 0.2f;    // 크리티컬 확률

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Attack | Ciritical")
    float CriticalFactor = 1.5f;    // 크리티컬 배율

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat | Attack | Ciritical")
    bool bIsCritical = false;       // 크리티컬 성패

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Attack | Logic")
	float AttackRange = 150.0f;     // 공격 범위

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Attack | Logic")
    float AttackRadius = 50.0f;     // 공격 반경

public:
    UFUNCTION(BlueprintCallable, Category = "Damage")
    void ApplyDamage(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Damage")
    float GetAttackCooldown() const { return 1.0f / FMath::Max(BaseAttackSpeed, 0.01f); }

    UFUNCTION(BlueprintCallable, Category = "Damage")
    float GetAttackPlayRate(float AnimationLength) const
    { return (AnimationLength < GetAttackCooldown()) ? 1.0f : (BaseAttackSpeed * AnimationLength); }

// --- 방어력 관련 스탯 ---
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Defense")
    float Defense = 5.f;

// --- 이동 속도 관련 스탯 ---
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Speed")
    float BaseSpeed = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Speed")
    float IdleSpeedFactor = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Speed")
    float PatrolSpeedFactor = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Speed")
    float ChaseSpeedFactor = 1.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Speed")
    float DeadSpeedFactor = 0.0f;

// --- 이동 거리 관련 수치 ---
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Distance")
    float PatrolRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Distance")
	float AttackableDistance = 100.0f;

// --- 벌목 관련 수치 ---

// --- 채광 관련 수치 ---

// --- AI Perception 관련 수치 ---
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | AI Perception")
    float SightRadius = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | AI Perception")
    float LoseSightRadius = 700.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | AI Perception")
    float PeripheralVisionAngleDegrees = 90.0f;

// --- Function ---
public:
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetSpeedForState(EMobState State);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetSpeedForState(EMobState State) const;

public:
    UFUNCTION(BlueprintCallable, Category = "Stamina")
    void ConsumeStamina(const float ConsumptionStamina);

private:
    void ResetSpeed();
    void ApplySpeedModifier(float SpeedMultiplier, float Duration);
    void SetSpeed(float NewSpeed);
};

