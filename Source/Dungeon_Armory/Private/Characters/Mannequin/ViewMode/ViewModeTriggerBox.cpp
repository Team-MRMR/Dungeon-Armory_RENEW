// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Mannequin/ViewMode/ViewModeTriggerBox.h"
#include "Characters/Mannequin/ViewMode/ViewModeComponent.h"
#include "GameFramework/Character.h"

class AManny;

AViewModeTriggerBox::AViewModeTriggerBox()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AViewModeTriggerBox::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &AViewModeTriggerBox::OnTriggerBeginOverlap);
    OnActorEndOverlap.AddDynamic(this, &AViewModeTriggerBox::OnTriggerEndOverlap);
}

void AViewModeTriggerBox::OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (!Character)
        return;

    UViewModeComponent* ViewModeComponent = Character->FindComponentByClass<UViewModeComponent>();
    if (ViewModeComponent)
    {
        ViewModeComponent->SetViewMode(EViewMode::FPS); // 실내로 진입
    }
}

void AViewModeTriggerBox::OnTriggerEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (!Character)
        return;

    UViewModeComponent* ViewModeComponent = Character->FindComponentByClass<UViewModeComponent>();
    if (ViewModeComponent)
    {
        ViewModeComponent->SetViewMode(EViewMode::TPS); // 실외로 나감
    }
}
