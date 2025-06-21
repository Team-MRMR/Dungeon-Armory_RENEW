// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Manager/ManagerBase.h"

#include "InGameTimeManager.generated.h"

UENUM(BlueprintType)
enum class EInGameTimePeriod : uint8
{
    Dawn,       // 새벽 (0시 ~ 6시)
    Morning,    // 아침 (6시 ~ 12시)
    Afternoon,  // 오후 (12시 ~ 18시)
    Night       // 밤   (18시 ~ 24시)
};

USTRUCT(BlueprintType)
struct FInGameTime
{
    GENERATED_BODY()

    /***** Variables *****/
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "In-Game Time")
    EInGameTimePeriod Period;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "In-Game Time")
    int32 Hours = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "In-Game Time")
    float Minutes = 0;

    /***** Functions *****/
public:
    // 시간을 증가시키는 함수
    void AddMinutes(float DeltaMinutes)
    {
        Minutes += DeltaMinutes;
        while (Minutes >= 60.0f)
        {
            Minutes -= 60.0f;
            Hours++;

            if (Hours >= 24)
            {
                Hours = 0; // 하루가 지나면 0으로 초기화
            }
        }

        if (0 <= Hours && Hours < 6)
        {
            Period = EInGameTimePeriod::Dawn;
        }
        else if (6 <= Hours && Hours < 12)
        {
            Period = EInGameTimePeriod::Morning;
        }
        else if (12 <= Hours && Hours < 18)
        {
            Period = EInGameTimePeriod::Afternoon;
        }
        else
        {
            Period = EInGameTimePeriod::Night;
        }
    }
};

class ANPCAIController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimePeriodChanged);

UCLASS()
class DUNGEON_ARMORY_API UInGameTimeManager : public UManagerBase
{
	GENERATED_BODY()

/***** Variables *****/
protected:
    FInGameTime CurrInGameTime;
    float TimeScale = 1.0f;

private:
    FTimerHandle TimerHandle;

/***** Functions (Unreal) *****/
public:
    UInGameTimeManager();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

/***** Functions (ManagerBase) *****/
public:
	static UInGameTimeManager* GetInstance();

/***** Functions (In-Game Time) *****/
public:
    UPROPERTY(BlueprintAssignable, Category = "In-Game Time")
    FOnTimePeriodChanged OnTimePeriodChanged;

    void StartTimer();
    void UpdateTime();

    UFUNCTION(BlueprintCallable, Category = "In-Game Time")
    FInGameTime GetCurrentInGameTime() const { return CurrInGameTime;};

    UFUNCTION(BlueprintCallable, Category = "In-Game Time")
    void SetTimeScale(float NewScale) { TimeScale = NewScale; }
};