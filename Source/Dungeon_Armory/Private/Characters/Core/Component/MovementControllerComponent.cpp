// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Core/Component/MovementControllerComponent.h"
#include "Characters/Core/Component/CharacterStatComponent.h"

#include "GameFramework/Character.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UMovementControllerComponent::UMovementControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UMovementControllerComponent::BeginPlay()
{
	Super::BeginPlay();

    AActor* ActorOwner = GetOwner();
    if (ActorOwner)
    {
        CharacterOwner = Cast<ACharacter>(ActorOwner);
        if (CharacterOwner)
        {
            MovementComponent = CharacterOwner->GetCharacterMovement();
            StatComponent = CharacterOwner->FindComponentByClass<UCharacterStatComponent>();
        }
    }
}

// Called every frame
void UMovementControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMovementControllerComponent::PatrolAtBase(const FVector& Destination, const float AcceptableRadius)
{
    if (!CharacterOwner || !MovementComponent)
        return;

	FVector CharacterLocation = CharacterOwner->GetActorLocation();
    FVector Direction = Destination - CharacterLocation;
    Direction = Direction.GetSafeNormal2D();

    float Distance = FVector::Dist2D(CharacterLocation, Destination);
    if (Distance <= AcceptableRadius)
    {
		OnMovementCompleted.Broadcast();
        return;
    }

    FVector Velocity = Direction * StatComponent->PatrolSpeedFactor;
    MovementComponent->AddInputVector(Velocity, false);
}

void UMovementControllerComponent::MoveToDestination(const FVector& Destination, const float AcceptableRadius)
{
    if (!CharacterOwner || !MovementComponent)
        return;

    FVector Direction = Destination - CharacterOwner->GetActorLocation();
    Direction = Direction.GetSafeNormal2D();

    float Distance = FVector::Dist2D(CharacterOwner->GetActorLocation(), Destination);
    if (Distance <= AcceptableRadius)
    {
        OnMovementCompleted.Broadcast();
        return;
    }

    FVector Velocity = Direction * StatComponent->PatrolSpeedFactor;
    MovementComponent->AddInputVector(Velocity, false);
}

void UMovementControllerComponent::RotateToTarget(const FVector& TargetLocation, float DeltaTime, float RotationSpeed)
{
    const auto Owner = GetOwner();
    if (!Owner)
        return;

    const FVector Direction = (TargetLocation - Owner->GetActorLocation()).GetSafeNormal2D();
    const FRotator TargetRotation = Direction.Rotation();
    const FRotator CurrentRotation = Owner->GetActorRotation();

    const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

    Owner->SetActorRotation(NewRotation);
}

void UMovementControllerComponent::StopMovement()
{
	if (MovementComponent)
	{
        // 이동 중단
        MovementComponent->StopMovementImmediately();

        //// 필요 시: 이동 입력 벡터 초기화 (AI Move가 아닌 사용자 입력 기반일 경우)
        //CharacterOwner->GetController()->StopMovement();
	}
}
