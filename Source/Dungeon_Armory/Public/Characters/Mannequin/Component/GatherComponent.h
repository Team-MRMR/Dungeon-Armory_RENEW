// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Characters/Mannequin/Interface/IToolEuipable.h"

#include "GatherComponent.generated.h"

class AGatherableActorBase;
class AManny;
class UAnimInstance;
class UAnimMontage;
class UCharacterStatComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEON_ARMORY_API UGatherComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGatherComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// --- 애니메이션 관련 ---
public:
	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LoggingMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MiningMontage;

protected:
	bool bIsHit;
	FHitResult HitResult;
	UAnimInstance* AnimInstance;
	UCharacterStatComponent* Stat;
	AManny* OwnerPlayerCharacter;

// --- 도구 관련 ---
public:
	EToolType ToolType;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Distance")
	float GatheringDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Logging")
	float LoggingDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Logging")
	float LoggingStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Logging")
	float LoggingSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Logging")
	float LoggingEfficiency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Mining")
	float MiningDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Mining")
	float MiningStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Mining")
	float MiningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Mining")
	float MiningEfficiency;

// --- 입력 관련 ---
private:
	bool bIsMontageEnded;		// 몽타주가 끝났는지 판별
	bool bCanReceiveInput;		// 입력을 받을 수 있는지 판별
	bool bHasNextGather;		// 선입력이 있는지 판별

public:
	void StartGather();
	void OnGather();
	void OnGatherEnd();
	void ReceiveInput();

private:
	void DoLineTrace(FHitResult& HitResult);
	void UpdateToolType();

	void Logging();
	void Mining();

	void ProceedGather();
	void PlayGatherMontage();
};
