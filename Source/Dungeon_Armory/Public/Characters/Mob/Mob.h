// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/Mob/MobBase.h"

#include "Mob.generated.h"

class UMobAttackComponent;

UCLASS()
class DUNGEON_ARMORY_API AMob : public AMobBase
{
	GENERATED_BODY()
	
	/***** Unreal *****/
public:
	AMob();

protected:
	virtual void BeginPlay() override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

// ----- Mob
protected:
	void CreateAttackComponent(UAttackComponentBase* NewAttackComponent);
	//void CreateAttackComponent(UAttackComponentBase* NewAttackComponent);
	//virtual void CreateAttackComponent_Implementation(UAttackComponentBase* NewAttackComponent) override { /*Empty Body*/ };
};
