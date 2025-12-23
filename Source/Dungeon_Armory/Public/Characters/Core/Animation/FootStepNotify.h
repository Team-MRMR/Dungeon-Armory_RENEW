#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "FootStepNotify.generated.h"

UENUM(BlueprintType)
enum class EFootstepFoot : uint8
{
    Left  UMETA(DisplayName = "Left Foot"),
    Right UMETA(DisplayName = "Right Foot"),
};

UCLASS(meta = (DisplayName = "Footstep Notify"))
class DUNGEON_ARMORY_API UFootStepNotify : public UAnimNotify
{
    GENERATED_BODY()

public:
    /** 좌 / 우 발 선택 */
    UPROPERTY(EditAnywhere, Category = "Footstep")
    EFootstepFoot Foot = EFootstepFoot::Left;

    /** 발 소켓 이름 */
    UPROPERTY(EditAnywhere, Category = "Footstep")
    FName LeftFootSocket = TEXT("foot_l");

    UPROPERTY(EditAnywhere, Category = "Footstep")
    FName RightFootSocket = TEXT("foot_r");

    /** 트레이스 길이 */
    UPROPERTY(EditAnywhere, Category = "Footstep")
    float TraceLength = 20.f;

    /** 최소 이동 속도 */
    UPROPERTY(EditAnywhere, Category = "Footstep")
    float MinSpeed = 10.f;

    /** 재생할 사운드 */
    UPROPERTY(EditAnywhere, Category = "Footstep")
    USoundBase* FootstepSound = nullptr;

    virtual void Notify(USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation) override;
};