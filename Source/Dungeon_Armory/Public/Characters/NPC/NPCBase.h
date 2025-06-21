// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"

#include "NPCBase.generated.h"

class UCharacterStatComponent;
class UTeamComponent;
class UMovementControllerComponent;
class ALocationPoint;

UCLASS()
class DUNGEON_ARMORY_API ANPCBase : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

/***** Unreal *****/
public:
	ANPCBase();

protected:
	virtual void BeginPlay() override;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

/***** Team Component *****/
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component | Stat")
	UCharacterStatComponent* StatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component | Team")
	UTeamComponent* TeamComponent;

public:
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

/***** Behavior Tree *****/
private:
	/** AI가 머물러 있는 포인트 */
	UPROPERTY(EditInstanceOnly, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	ALocationPoint* HomePoint;

	/** AI가 경로를 순회할 때 사용할 이동 포인트 */
	UPROPERTY(EditInstanceOnly, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	TArray<ALocationPoint*> LocationPoints;

	/** 현재 이동 지점 인덱스 */
	int32 CurrLocationPointIndex = 0;

public:
	FVector GetNextPoint();
	FVector GetHomePoint() const;

};
