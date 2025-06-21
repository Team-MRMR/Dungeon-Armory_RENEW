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

void UGatherComponent::OnGather()
{
    DoLineTrace(HitResult);

    // 1. 라인 트레이스를 통해 감지한 대상 검사
    AActor* Target = HitResult.GetActor();
    if (!Target)
        return;

    // 2. 플레이어의 도구와 자원 액터의 타입을 가져오기 위한 준비
    IIToolEuipable* IToolEuipable = Cast<IIToolEuipable>(GetOwner());
	if (!IToolEuipable)
		return;

    AGatherableActorBase* GatherableActor = Cast<AGatherableActorBase>(Target);
	if (!GatherableActor)
		return;

    // 3. 대상에 따라 분기 처리
	EResourceType ResourceType = GatherableActor->GetResourceType();      // 자원 액터에서 ResourceType을 가져옴
    if (ToolType == EToolType::Axe && ResourceType == EResourceType::Tree)
    {
        Logging();
    }
    else if(ToolType == EToolType::Pickaxe && ResourceType == EResourceType::Vein)
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
    // 1. 플레이어 또는 컴포넌트 오너 얻기
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
        return;

    // 2. 라인 트레이스의 시작점과 끝점 계산
    FVector Start, End;
    FRotator ViewRot;
    OwnerActor->GetActorEyesViewPoint(Start, ViewRot);
    End = Start + ViewRot.Vector() * GatheringDistance;

    // 3. 충돌 파라미터 설정
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(OwnerActor); // 자신은 무시

    // 4. 실제 라인 트레이스 수행
    bIsHit = GetWorld()->LineTraceSingleByChannel(
        OutHitResult,
        Start,
        End,
        ECC_Visibility, // 또는 커스텀 채널: ECC_GameTraceChannel1 등
        TraceParams
    );

    // 5. 디버그용 선 그리기 (테스트 시에만)
#if WITH_EDITOR
    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f);
    if (OutHitResult.bBlockingHit)
    {
        DrawDebugSphere(GetWorld(), OutHitResult.ImpactPoint, 5.0f, 12, FColor::Red, false, 0.1f);
    }
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
                const float DamageAmount = Stat->BaseAttackDamage;
                DamagedActor->ReceiveDamage(DamageAmount);

                const float ConsumptionStamina = Stat->Stamina.LoggingConsumption;
                Stat->ConsumeStamina(ConsumptionStamina); // 스태미너 소비

                OwnerPlayerCharacter->DecreaseDurability();  // 도구 내구도 감소

                PlayGatherMontage();

                return;
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
                const float DamageAmount = Stat->BaseAttackDamage;
                DamagedActor->ReceiveDamage(DamageAmount);

                const float ConsumptionStamina = Stat->Stamina.MiningConsumption;
                Stat->ConsumeStamina(ConsumptionStamina); // 스태미너 소비

                OwnerPlayerCharacter->DecreaseDurability();  // 도구 내구도 감소

                return;
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
        const float ConsumptionStamina = Stat->Stamina.LoggingConsumption;
        const float CurrentStamina = Stat->Stamina.GetCurrent();

        // 현재 스태미너가 소비 스태미너보다 작으면 공격할 수 없음
        if (CurrentStamina <= ConsumptionStamina)
            return;

        AnimInstance->Montage_Play(LoggingMontage);
    }
    else if (ToolType == EToolType::Pickaxe)
    {
        const float ConsumptionStamina = Stat->Stamina.MiningConsumption;
        const float CurrentStamina = Stat->Stamina.GetCurrent();

        // 현재 스태미너가 소비 스태미너보다 작으면 공격할 수 없음
        if (CurrentStamina <= ConsumptionStamina)
            return;
        AnimInstance->Montage_Play(MiningMontage);
    }
    
    bIsMontageEnded = false;
}
