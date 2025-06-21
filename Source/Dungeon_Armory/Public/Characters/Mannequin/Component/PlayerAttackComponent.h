// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Core/Component/AttackComponentBase.h"
#include "PlayerAttackComponent.generated.h"

class AManny;
class UAnimMontage;
class UCharacterStatComponent;

/**
 * 
 */
UCLASS()
class DUNGEON_ARMORY_API UPlayerAttackComponent : public UAttackComponentBase
{
	GENERATED_BODY()

/***** Attack *****/
private:
    UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* ComboAttackMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
    TArray<FName> ComboAttackSections;

    AManny* OwnerPlayerCharacter;
    UAnimInstance* AnimInstance;
    UCharacterStatComponent* Stat;

    int32 CurrentComboIndex; // 현재 콤보 인덱스

    bool bIsMontageEnded;   // 몽타주 종료 여부
    bool bCanReceiveInput;  // 콤보 입력 가능 여부
    bool bNextCombo;        // 다음 콤보 진행 여부

    /***** Unreal *****/
public:
    UPlayerAttackComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    /***** Attack *****/
public:
    void StartAttack()override;     // 외부에서 공격 시작 시 호출
    void OnAttack() override;        // AttackNotify에서 호출
    void OnAttackEnd() override;
    void ReceiveInput();    // 콤보 입력 수신

protected:
    void ProceedCombo();
    void PlayComboAttackMontage(int32 ComboIndex);

private:
    virtual float CalculateDamage(UCharacterStatComponent* Attacker, UCharacterStatComponent* Defender);
};
