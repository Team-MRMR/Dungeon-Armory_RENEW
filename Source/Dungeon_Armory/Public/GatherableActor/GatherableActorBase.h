// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Characters/Core/Interface/IDamageable.h"

#include "GatherableActorBase.generated.h"

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Other    UMETA(DisplayName = "Other"),

	Tree	 UMETA(DisplayName = "Tree"),
	Vein     UMETA(DisplayName = "Vein")
};

class USoundBase;

UCLASS()
class DUNGEON_ARMORY_API AGatherableActorBase : public AActor, public IIDamageable
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGatherableActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	EResourceType GetResourceType() const { return ResourceType; }

// --- GatherableActorBase ---
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	EResourceType ResourceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float CurrentHealth;

protected:
	// 피격 및 사망 시 재생할 사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* DieSound;


// IIDamageable을(를) 통해 상속됨
public:
	void ReceiveDamage(float DamageAmount) override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Die")
	void Die() override;
	virtual void Die_Implementation() { }

};
