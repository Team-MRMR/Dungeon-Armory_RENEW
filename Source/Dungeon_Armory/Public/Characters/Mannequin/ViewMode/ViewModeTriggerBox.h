#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"

#include "ViewModeTriggerBox.generated.h"

class UViewModeComponent;

UCLASS()
class DUNGEON_ARMORY_API AViewModeTriggerBox : public ATriggerBox
{
    GENERATED_BODY()

public:
    AViewModeTriggerBox();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION()
    void OnTriggerEndOverlap(AActor* OverlappedActor, AActor* OtherActor);
};