// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"

#include "Characters/NPC/AI/LocationPoint.h"

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
public:
	void SetIsShopping(bool _bIsShopping) { bIsShopping = _bIsShopping; }
	FORCEINLINE bool GetIsShopping() const { return bIsShopping; }

	FORCEINLINE ALocationPoint* const GetPoint(bool bIsRandom);

private:
	/** 대장간 내부에서 랜덤하게 돌아다닐 변수 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	bool bIsShopping = false;

	/** AI가 머물러 있는 포인트 */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	ALocationPoint* HomePoint;

	/** AI가 경로를 순회할 때 사용할 이동 포인트 */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	TArray<ALocationPoint*> LocationPoints;

	/** 현재 이동 지점 인덱스 */
	int32 CurrentIndex = 0;
};
