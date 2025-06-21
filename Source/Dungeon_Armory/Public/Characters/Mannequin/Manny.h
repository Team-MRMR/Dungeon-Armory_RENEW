// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Characters/Core/AI/Team/TeamComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "Characters/Core/Interface/IDamageable.h"
#include "Characters/Mannequin/Interface/IToolEuipable.h"

#include "Manny.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UViewModeComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UInteractionComponent;
class UCharacterStatComponent;
class UPlayerAttackComponent;
class UGatherComponent;


UCLASS()
class DUNGEON_ARMORY_API AManny : public ACharacter, public IGenericTeamAgentInterface, public IIDamageable, public IIToolEuipable
{
	GENERATED_BODY()

/***** Camera *****/
private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraSpringArm;

	/** TPS camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "View", meta = (AllowPrivateAccess = "true"))
	UViewModeComponent* ViewModeComponent;

/****** Team ******/
private:
	/** AI Perception Stimuli Source */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;

	/** Team Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Team, meta = (AllowPrivateAccess = "true"))
	UTeamComponent* TeamComponent;

/***** Input *****/
private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Core", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* CoreContext;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Battle", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* BattleContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Core", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Core", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Core", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Core", meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Battle", meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

/***** Interaction *****/
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	UInteractionComponent* InteractionComponent;

/***** Stat *****/
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	UCharacterStatComponent* StatComponent;

/***** Attack *****/
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	UPlayerAttackComponent* AttackComponent;

/***** Tool Stat *****/
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	UGatherComponent* GatherComponent;

// ***** Animation *****/
private:
	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, Category = "Hit", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Hit1Montage;

	UPROPERTY(EditAnywhere, Category = "Hit", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Hit2Montage;

/***** Unreal *****/
public:
	// Sets default values for this character's properties
	AManny();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

/***** Team *****/
	/** IGenericTeamAgentInterface implementation */
	virtual FGenericTeamId GetGenericTeamId() const override;

	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

/***** Camera *****/
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraSpringArm; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return CameraComponent; }

/***** Input *****/
protected:
	/** Called for moving input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Interacting input */
	void Interact(const FInputActionValue& Value);

	/** Called for Attacking input */
	void LeftClickAction(const FInputActionValue& Value);

/***** Damage*****/
public:
	// IIDamageable을(를) 통해 상속됨
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Hit & Die")
	void ReceiveDamage(float DamageAmount);
	virtual void ReceiveDamage_Implementation(float DamageAmount);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Hit & Die")
	void Die();
	virtual void Die_Implementation() { }

/***** Utilities *****/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Durability")
	void DecreaseDurability();
	virtual void DecreaseDurability_Implementation() { }

	virtual EToolType GetToolType_Implementation() const override { return EToolType::Other; }
};
