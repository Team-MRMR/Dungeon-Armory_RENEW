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

UENUM(BlueprintType)
enum class NPCState : uint8
{
	None		UMETA(DisplayName = "None"),		// 상태 없음

	Roaming		UMETA(DisplayName = "Roaming"),		// 돌아다니는 상태
	Returning	UMETA(DisplayName = "Returning"),	// 돌아가는 상태

	Shopping	UMETA(DisplayName = "Shopping"),	// 쇼핑하는 상태
	Paying		UMETA(DisplayName = "Paying"),		// 계산하는 상태
	Exiting		UMETA(DisplayName = "Exiting"),		// 나가는 상태
};

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

/***** Stat Component *****/
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component | Stat")
	UCharacterStatComponent* StatComponent;

/***** Team Component *****/
public:	

public:
	virtual FGenericTeamId GetGenericTeamId() const override;

/***** Behavior Tree *****/
public:
	UFUNCTION(BlueprintCallable, Category = "Behavior Tree")
	void SetCurrentState(NPCState NewState);
	FORCEINLINE NPCState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintCallable, Category = "Behavior Tree")
	void SetIsShopping(bool _bIsShopping);
	FORCEINLINE bool GetIsShopping() const { return bIsShopping; }

	FVector const GetPointLocation();

	/** 나가기 행동 위치 */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Behavior Tree")
	ALocationPoint* ExitPoint;

	/** 계산 행동 위치 */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Behavior Tree")
	ALocationPoint* PayPoint;

	/** 쇼핑 행동 랜덤 위치 */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Behavior Tree")
	TArray<ALocationPoint*> ShoppingPoints;

	/** AI가 마지막으로 돌아가는 포인트 */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Behavior Tree")
	ALocationPoint* ReturnPoint;

	/** AI가 경로를 순회할 때 사용할 이동 포인트 */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Behavior Tree")
	TArray<ALocationPoint*> RoammingPoints;

private:
	/** 대장간 내부에서 랜덤하게 돌아다닐 변수 */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	bool bIsShopping = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	NPCState CurrentState = NPCState::None;

	/** 현재 이동 지점 인덱스 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	int32 CurrentShoppingIndex = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	int32 CurrentRommaingIndex = 0;
};
