#include "Characters/Core/Animation/FootStepNotify.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// sound
#include "Kismet/GameplayStatics.h"

void UFootStepNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp)
        return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner)
        return;

    UWorld* World = Owner->GetWorld();
    if (!World || !FootstepSound)
        return;

    const APawn* PawnOwner = Cast<APawn>(Owner);
    if (!PawnOwner)
        return;

    const FVector Velocity = PawnOwner->GetVelocity();
    const float Speed2D = FVector(Velocity.X, Velocity.Y, 0.f).Size();

    if (Speed2D < MinSpeed)
        return;

    const ACharacter* Character = Cast<ACharacter>(Owner);
    if (Character && Character->GetCharacterMovement()->IsFalling())
        return;

    const FName FootSocketName =
        (Foot == EFootstepFoot::Left) ? LeftFootSocket : RightFootSocket;

    if (!MeshComp->DoesSocketExist(FootSocketName))
        return;

    const FVector Start = MeshComp->GetSocketLocation(FootSocketName);
    const FVector End = Start - FVector(0.f, 0.f, TraceLength);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

    const bool bHit = World->LineTraceSingleByChannel(
        Hit,
        Start,
        End,
        ECC_Visibility,
        Params
    );

    if (!bHit)
        return;

    UGameplayStatics::PlaySoundAtLocation(
        World,
        FootstepSound,
        Hit.ImpactPoint
    );

#if WITH_EDITOR
    DrawDebugLine(
        World,
        Start,
        End,
        bHit ? FColor::Green : FColor::Red,
        false,
        1.0f
    );
#endif
}
