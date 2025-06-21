#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "HealthStat.generated.h"

UCLASS(Blueprintable)
class DUNGEON_ARMORY_API UHealthStat : public UObject
{
    GENERATED_BODY()

public:
    /** 최대 스태미너 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float Max = 100.f;

    /** 회복 속도 (초당 회복량) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float RegenRate = 5.f;

    /** 회복 체크 주기 (초) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float RegenSpeed = 1.0f;

public:
    /** 월드 설정 및 초기화 */
    void Initialize(UObject* WorldContextObject);

    /** 스태미너 소비 */
    void Consume(float Amount);

    /** 스태미너 회복 타이머 종료 */
    UFUNCTION()
    void StopConsume();

    /** 회복 처리 */
    void ApplyRegen(double CurrentTime);

    /** 현재 스태미너 반환 */
    float GetStamina();

    /** 현재 스태미너 수치 직접 반환 */
    float GetCurrent() const;

    /** 최대 스태미너로 회복 */
    void RestoreFull();

    /** 고갈 상태 확인 */
    bool IsDepleted() const;

private:
    float Current = 100.f;
    double LastConsumedTime = 0.0;
    double LastCalculatedTime = 0.0;
    bool bIsBeingConsumed = false;

    UWorld* World = nullptr;
    FTimerHandle ConsumeTimerHandle;
};
