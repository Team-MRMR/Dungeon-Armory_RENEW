// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Mannequin/ViewMode/ViewModeComponent.h"

#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

UViewModeComponent::UViewModeComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UViewModeComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        SpringArm = OwnerCharacter->FindComponentByClass<USpringArmComponent>();
        
        Camera = OwnerCharacter->FindComponentByClass<UCameraComponent>();
		if (Camera)
		{
			// 카메라의 초기 위치를 저장
			Camera->GetRelativeLocation() = FPSCameraPosition;
		}

        Mesh = OwnerCharacter->GetMesh();
    }
}

void UViewModeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UpdateViewMode(DeltaTime);
}

void UViewModeComponent::SetIndoorState(bool bIndoor)
{
    EViewMode DesiredMode = bIndoor ? EViewMode::FPS : EViewMode::TPS;

    if (TargetViewMode != DesiredMode)
    {
        bIsIndoor = bIndoor;
        TargetViewMode = DesiredMode;
        InterpAlpha = 0.0f;
    }
}

void UViewModeComponent::UpdateViewMode(float DeltaTime)
{
    FVector TargetCameraPos = (TargetViewMode == EViewMode::FPS) ? FPSCameraPosition : TPSCameraPosition;
    float TargetArmLength = (TargetViewMode == EViewMode::FPS) ? FPSTargetArmLength : TPSTargetArmLength;

    FVector CurrentCameraPos = Camera->GetRelativeLocation();
    float CurrentArmLength = SpringArm->TargetArmLength;

    bool bViewModeChanged = (CurrentViewMode != TargetViewMode);

    const float Tolerance = 1.0f;
    bool bNeedCorrection =
        !CurrentCameraPos.Equals(TargetCameraPos, Tolerance) ||
        !FMath::IsNearlyEqual(CurrentArmLength, TargetArmLength, Tolerance);

    if (bViewModeChanged || bNeedCorrection)
    {
        InterpAlpha += DeltaTime * InterpSpeed;
        InterpAlpha = FMath::Clamp(InterpAlpha, 0.0f, 1.0f);

        // Lerp 적용 (현재 위치와 목표 위치 사이를 보간)
        FVector NewCameraPos = FMath::Lerp(CurrentCameraPos, TargetCameraPos, InterpAlpha);
        float NewArmLength = FMath::Lerp(CurrentArmLength, TargetArmLength, InterpAlpha);

        Camera->SetRelativeLocation(NewCameraPos);
        SpringArm->TargetArmLength = NewArmLength;

        // 메시 렌더링 설정
        if (Mesh)
        {
            if (InterpAlpha > FPSRenderOffTiming && TargetViewMode == EViewMode::FPS)
            {
                Mesh->SetOwnerNoSee(false);
            }
            else if (InterpAlpha > TPSRenderOffTiming && TargetViewMode == EViewMode::TPS)
            {
                Mesh->SetOwnerNoSee(false);
            }
        }

        if (InterpAlpha >= 1.0f)
        {
            // 보정 완료
            CurrentViewMode = TargetViewMode;
            InterpAlpha = 0.f; // 다음 보간을 위해 초기화
        }
    }
}

void UViewModeComponent::ApplyCameraTransform(float Alpha)
{
    if (!Camera)
        return;

    FVector FromPos = (CurrentViewMode == EViewMode::FPS) ? FPSCameraPosition : TPSCameraPosition;
    FVector ToPos = (TargetViewMode == EViewMode::FPS) ? FPSCameraPosition : TPSCameraPosition;

    FVector NewCameraPosition = FMath::Lerp(FromPos, ToPos, Alpha);
    Camera->SetRelativeLocation(NewCameraPosition);
}

void UViewModeComponent::ApplySpringArmTransform(float Alpha)
{
    if (!SpringArm)
        return;

    float FromLength = (CurrentViewMode == EViewMode::FPS) ? FPSTargetArmLength : TPSTargetArmLength;
    float ToLength = (TargetViewMode == EViewMode::FPS) ? FPSTargetArmLength : TPSTargetArmLength;

    float NewLength = FMath::Lerp(FromLength, ToLength, Alpha);
    SpringArm->TargetArmLength = NewLength;
}