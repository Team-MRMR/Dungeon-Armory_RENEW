// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Interact/InteractionComponent.h"
#include "Core/Interact/InteractableComponent.h"
#include "Core/Interact/Interface/Interactable.h"

#include "Characters/Mannequin/Manny.h"

#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    InteractableTarget = nullptr;
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
    OwnerActor = GetOwner();
}

// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    TraceForInteractable();
}

void UInteractionComponent::TryInteract()
{
    if (InteractableTarget)
    {
        IInteractable* InteractableActor = Cast<IInteractable>(InteractableTarget);
        if (InteractableActor && IInteractable::Execute_CanInteract(InteractableTarget, GetOwner()))
        {
			UE_LOG(LogTemp, Warning, TEXT("UInteractionComponent::TryInteract() Called"));
            InteractableActor->IInteractable::Execute_Interact(InteractableTarget, GetOwner());
        }
    }
}

void UInteractionComponent::TraceForInteractable()
{
    if (!OwnerActor)
        return;

    FVector Start, End;
    FRotator ViewRot;
    OwnerActor->GetActorEyesViewPoint(Start, ViewRot);
    End = Start + ViewRot.Vector() * TraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerActor);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
    AActor* HitActor = bHit ? Hit.GetActor() : nullptr;
    

    DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Red : FColor::Green, false, 0.01f, 0, 2.0f);

    // 이미 동일한 대상이면 무시
    if (HitActor == InteractableTarget)
        return;


    // 이전 강조 해제
    if (InteractableTarget)
    {
        IInteractable::Execute_DisableOutline(InteractableTarget);
    }

    InteractableTarget = nullptr;

    // 새 대상 설정
    IInteractable* InteractableActor = Cast<IInteractable>(HitActor);
    if (InteractableActor)
    {
        InteractableTarget = HitActor;
        IInteractable::Execute_EnableOutline(HitActor);
    }
}
