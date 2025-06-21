// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttackNotify.generated.h"

// 공격 판정 발생 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnAttack);

/**
 * 
 */
UCLASS()
class DUNGEON_ARMORY_API UAttackNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
