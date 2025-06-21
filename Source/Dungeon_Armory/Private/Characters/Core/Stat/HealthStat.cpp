// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Core/Stat/HealthStat.h"
#include "Engine/World.h"

void UHealthStat::Initialize(UObject* WorldContextObject)
{
    World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;

    Current = Max;
    LastConsumedTime = 0.0;
    LastCalculatedTime = 0.0;
    bIsBeingConsumed = false;
}

void UHealthStat::Consume(float Amount)
{
    if (!World) return;

    const double CurrentTime = World->GetTimeSeconds();

    ApplyRegen(CurrentTime);
    Current = FMath::Clamp(Current - Amount, 0.f, Max);
    LastConsumedTime = CurrentTime;
    bIsBeingConsumed = true;

    // 타이머 초기화 (최근 소비 기준으로)
    World->GetTimerManager().SetTimer(
        ConsumeTimerHandle,
        this,
        &UHealthStat::StopConsume,
        RegenSpeed,
        false
    );
}

void UHealthStat::StopConsume()
{
    bIsBeingConsumed = false;
}

void UHealthStat::ApplyRegen(double CurrentTime)
{
    if (!World) return;

    if (bIsBeingConsumed)
    {
        LastCalculatedTime = CurrentTime;
        return;
    }

    const double Delta = CurrentTime - LastCalculatedTime;
    if (Delta <= 0.0) return;

    const float RegenAmount = RegenRate * Delta;
    Current = FMath::Clamp(Current + RegenAmount, 0.f, Max);
    LastCalculatedTime = CurrentTime;
}

float UHealthStat::GetStamina()
{
    if (!World) return Current;

    const double CurrentTime = World->GetTimeSeconds();
    ApplyRegen(CurrentTime);
    return Current;
}

float UHealthStat::GetCurrent() const
{
    return Current;
}

void UHealthStat::RestoreFull()
{
    Current = Max;
}

bool UHealthStat::IsDepleted() const
{
    return Current <= 0.f;
}

