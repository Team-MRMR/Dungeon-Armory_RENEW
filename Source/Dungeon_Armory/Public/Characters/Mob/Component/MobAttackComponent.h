// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Characters/Core/Component/AttackComponentBase.h"

#include "MobAttackComponent.generated.h"

class UAnimMontage;
class UCharacterStatComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEON_ARMORY_API UMobAttackComponent : public UAttackComponentBase
{
	GENERATED_BODY()

/***** Unreal *****/
public:	
	// Sets default values for this component's properties
	UMobAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/***** Attack *****/
private:
	UPROPERTY(EditDefaultsOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* NormalAttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CriticalAttackMontage;

	float ElapsedTime;
	bool bCanAttack;
	bool bIsStartedAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	bool bIsEndedAttack;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	bool bIsCritical;

public:
	void StartAttack() override;
	bool GetCanAttack() const;
	void OnAttack() override;
	void OnAttackEnd() override;

private:
	virtual float CalculateDamage(UCharacterStatComponent* Attacker, UCharacterStatComponent* Defender);

};
