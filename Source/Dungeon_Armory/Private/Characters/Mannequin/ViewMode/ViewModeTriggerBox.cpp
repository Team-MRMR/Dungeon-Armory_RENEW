// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Mannequin/ViewMode/ViewModeTriggerBox.h"
#include "Characters/Mannequin/ViewMode/ViewModeComponent.h"
#include "GameFramework/Character.h"
#include "Characters/Mannequin/Manny.h"

AViewModeTriggerBox::AViewModeTriggerBox()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AViewModeTriggerBox::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &AViewModeTriggerBox::OnTriggerBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AViewModeTriggerBox::OnTriggerEndOverlap);

    // 다음 프레임에 검사
    GetWorldTimerManager().SetTimerForNextTick(this, &AViewModeTriggerBox::CheckInitialOverlap);
}

void AViewModeTriggerBox::CheckInitialOverlap()
{
   TArray<AActor*> OverlappingActors;
   GetOverlappingActors(OverlappingActors, AManny::StaticClass());

   for (AActor* Actor : OverlappingActors)
   {
       UViewModeComponent* ViewModeComponent = Actor->FindComponentByClass<UViewModeComponent>();
       if (ViewModeComponent)
       {
           ViewModeComponent->SetViewMode(EViewMode::FPS);
           return;
       }
   }
}

void AViewModeTriggerBox::OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    UViewModeComponent* ViewModeComponent = OtherActor->FindComponentByClass<UViewModeComponent>();
    if (ViewModeComponent)
    {
		UE_LOG(LogTemp, Warning, TEXT("Enter Trigger Box"));
        ViewModeComponent->SetViewMode(EViewMode::FPS); // 실내로 진입
    }
}

void AViewModeTriggerBox::OnTriggerEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    UViewModeComponent* ViewModeComponent = OtherActor->FindComponentByClass<UViewModeComponent>();
    if (ViewModeComponent)
    {
		UE_LOG(LogTemp, Warning, TEXT("Exit Trigger Box"));
        ViewModeComponent->SetViewMode(EViewMode::TPS); // 실외로 나감
    }
}
