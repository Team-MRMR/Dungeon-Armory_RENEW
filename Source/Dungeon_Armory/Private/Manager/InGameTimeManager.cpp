// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/InGameTimeManager.h"

UInGameTimeManager::UInGameTimeManager()
{

}

void UInGameTimeManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TManagerBase<UInGameTimeManager>::InitializeInstance(this);

	StartTimer();
}

void UInGameTimeManager::Deinitialize()
{
    Super::Deinitialize();

    // 타이머 정리
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    }
}

UInGameTimeManager* UInGameTimeManager::GetInstance()
{
	auto Instance = TManagerBase<UInGameTimeManager>::GetInstance();
	if (Instance)
	{
		return Instance;
	}

	return nullptr;
}

void UInGameTimeManager::StartTimer()
{
    if (GetWorld())
    {
        // 일정 주기로 UpdateTime() 호출 (1초마다 실행)
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInGameTimeManager::UpdateTime, 1.0f, true);
    }
}

void UInGameTimeManager::UpdateTime()
{
    // 시간 누적 계산 (Time Scale)
    CurrInGameTime.AddMinutes(TimeScale);

	//if (3.0f <= CurrInGameTime.Minutes && CurrInGameTime.Minutes < 4.0f)
    //{
    //    // 시간 변경 이벤트 호출
    //    OnTimePeriodChanged.Broadcast();
    //}

}
