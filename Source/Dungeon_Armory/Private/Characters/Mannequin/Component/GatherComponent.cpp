// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mannequin/Component/GatherComponent.h"
#include "Characters/Core/Component/CharacterStatComponent.h"
#include "Characters/Mannequin/Interface/IToolEuipable.h"
#include "Characters/Mannequin/Manny.h"

#include "GatherableActor/GatherableActorBase.h"

// Sets default values for this component's properties
UGatherComponent::UGatherComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

    bIsMontageEnded = true;
    bCanReceiveInput = true;
    bHasNextGather = false;
}

// Called when the game starts
void UGatherComponent::BeginPlay()
{
	Super::BeginPlay();
	
    OwnerPlayerCharacter = Cast<AManny>(GetOwner());
    if (OwnerPlayerCharacter)
    {
        AnimInstance = OwnerPlayerCharacter->GetMesh()->GetAnimInstance();
        Stat = OwnerPlayerCharacter->GetComponentByClass<UCharacterStatComponent>();
    }
}


// Called every frame
void UGatherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UGatherComponent::StartGather()
{
    UAnimInstance* LatestAnimInstance = OwnerPlayerCharacter->GetMesh()->GetAnimInstance();
    if (AnimInstance != LatestAnimInstance)
    {
        AnimInstance = LatestAnimInstance;
    }

    const float ConsumptionStamina = Stat->Stamina.AttackConsumption;
    const float CurrentStamina = Stat->Stamina.GetCurrent();

    // 현재 스태미너가 소비 스태미너보다 작으면
    if (CurrentStamina <= ConsumptionStamina)
        return;

    DoLineTrace(HitResult);

    AActor* Target = HitResult.GetActor();
    if (!Target)
        return;

    AGatherableActorBase* GatherableActor = Cast<AGatherableActorBase>(Target);
    if (!GatherableActor)
        return;

    EResourceType ResourceType = GatherableActor->GetResourceType();      // 자원 액터에서 ResourceType을 가져옴

    UpdateToolType();

    if ((ToolType == EToolType::Axe && ResourceType == EResourceType::Tree)
        || (ToolType == EToolType::Pickaxe && ResourceType == EResourceType::Vein))
    {
        if (bCanReceiveInput)
        {
            bHasNextGather = true;
            bCanReceiveInput = false;
        }

        if (bIsMontageEnded)
        {
            ProceedGather();
        }
    }
}

void UGatherComponent::OnGather()
{
    if (ToolType == EToolType::Axe)
    {
        Logging();
    }
    else if(ToolType == EToolType::Pickaxe)
    {
        Mining();
    }
}

void UGatherComponent::OnGatherEnd()
{
    bIsMontageEnded = true;
    bCanReceiveInput = true;

    if (bHasNextGather)
    {
        ProceedGather();
        bHasNextGather = false;
    }
}

void UGatherComponent::ReceiveInput()
{
    bCanReceiveInput = true;
}

void UGatherComponent::DoLineTrace(FHitResult& OutHitResult)
{
    FVector Start, End;
    FRotator ViewRot;
    GetOwner()->GetActorEyesViewPoint(Start, ViewRot);
    End = Start + ViewRot.Vector() * Stat->AttackableDistance;

    // 3. 충돌 파라미터 설정
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(GetOwner()); // 자신은 무시

    // 4. 실제 스윕 트레이스 수행
    bIsHit = GetWorld()->SweepSingleByChannel(
        OutHitResult,
        Start,
        End,
        FRotationMatrix::MakeFromZ(End - Start).ToQuat(),
        ECC_Visibility,
        FCollisionShape::MakeCapsule(
            Stat->AttackRadius,
            Stat->AttackRange * 0.5f
        ),
        TraceParams
    );

    // 5. 디버그용 선 그리기 (테스트 시에만)
#if WITH_EDITOR
    DrawDebugCapsule(
        GetWorld(),
        (Start + End) * 0.5f,
        Stat->AttackRange * 0.5f,
        Stat->AttackRadius,
        FRotationMatrix::MakeFromZ(End - Start).ToQuat(),
        bIsHit ? FColor::Red : FColor::Green,
        false,
        0.5f
    );
#endif
}

void UGatherComponent::UpdateToolType()
{
    auto OwnerActor = GetOwner();
    if (!OwnerActor)
        return;

    auto IToolEuipable = Cast<IIToolEuipable>(OwnerActor);
    if (!IToolEuipable)
        return;

    ToolType = IToolEuipable->Execute_GetToolType(OwnerActor);
}

void UGatherComponent::Logging()
{
    if (bIsHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (!HitActor)
            return;

        auto GatherableActor = Cast<AGatherableActorBase>(HitActor);
        if (GatherableActor)
        {
            IIDamageable* DamagedActor = Cast<IIDamageable>(GatherableActor);
            if (DamagedActor && Stat)
            {
                const float DamageAmount = Stat->LoggingDamage;
                DamagedActor->Execute_ReceiveDamage(HitActor, DamageAmount);

                OwnerPlayerCharacter->Execute_DecreaseDurability(OwnerPlayerCharacter);  // 도구 내구도 감소
                const float ConsumptionStamina = Stat->Stamina.LoggingConsumption;
                Stat->ConsumeStamina(ConsumptionStamina); // 스태미너 소비
            }

        }
    }
}

void UGatherComponent::Mining()
{
    if (bIsHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (!HitActor)
            return;

        auto GatherableActor = Cast<AGatherableActorBase>(HitActor);
        if (GatherableActor)
        {
            IIDamageable* DamagedActor = Cast<IIDamageable>(GatherableActor);
            if (DamagedActor && Stat)
            {
                const float DamageAmount = Stat->MiningDamage;
                DamagedActor->Execute_ReceiveDamage(HitActor, DamageAmount);

                OwnerPlayerCharacter->Execute_DecreaseDurability(OwnerPlayerCharacter);  // 도구 내구도 감소
                const float ConsumptionStamina = Stat->Stamina.MiningConsumption;
                Stat->ConsumeStamina(ConsumptionStamina); // 스태미너 소비
            }
        }
    }
}

void UGatherComponent::ProceedGather()
{
    UpdateToolType();

    PlayGatherMontage();

    bHasNextGather = false;
}

void UGatherComponent::PlayGatherMontage()
{
    if (!AnimInstance)
        return;

    if (!LoggingMontage || !MiningMontage)
        return;

    if (AnimInstance->Montage_IsPlaying(LoggingMontage) || AnimInstance->Montage_IsPlaying(MiningMontage))
        return;

    if (ToolType == EToolType::Axe)
    {
        AnimInstance->Montage_Play(LoggingMontage);
    }
    else if (ToolType == EToolType::Pickaxe)
    {
        AnimInstance->Montage_Play(MiningMontage);
    }

    bIsMontageEnded = false;
}
