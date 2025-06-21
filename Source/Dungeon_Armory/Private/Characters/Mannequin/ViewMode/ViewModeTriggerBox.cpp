// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Mannequin/ViewMode/ViewModeTriggerBox.h"
#include "Characters/Mannequin/ViewMode/ViewModeComponent.h"
#include "GameFramework/Character.h"

AViewModeTriggerBox::AViewModeTriggerBox()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AViewModeTriggerBox::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &AViewModeTriggerBox::OnTriggerBeginOverlap);
    OnActorEndOverlap.AddDynamic(this, &AViewModeTriggerBox::OnTriggerEndOverlap);

    //// --- 수동으로 Overlapping 상태 확인 및 처리 ---
    //TArray<AActor*> OverlappingActors;
    //GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

    //for (AActor* Actor : OverlappingActors)
    //{
    //    ACharacter* Character = Cast<ACharacter>(Actor);
    //    if (!Character)
    //        continue;

    //    UViewModeComponent* ViewModeComponent = Character->FindComponentByClass<UViewModeComponent>();
    //    if (ViewModeComponent)
    //    {
    //        ViewModeComponent->SetIndoorState(true); // 시작 시 트리거 안에 있으면 실내로 설정
    //    }
    //}
}

void AViewModeTriggerBox::OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (!Character)
        return;

    UViewModeComponent* ViewModeComponent = Character->FindComponentByClass<UViewModeComponent>();
    if (ViewModeComponent)
    {
        ViewModeComponent->SetIndoorState(true); // 실내로 진입
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
        ViewModeComponent->SetIndoorState(false); // 실외로 나감
    }
}
