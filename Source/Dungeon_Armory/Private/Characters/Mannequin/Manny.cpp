// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mannequin/Manny.h"
#include "Characters/Mannequin/ViewMode/ViewModeComponent.h"
#include "Characters/Mannequin/Component/GatherComponent.h"

#include "Characters/Mannequin/Component/PlayerAttackComponent.h"
#include "Characters/Core/Component/CharacterStatComponent.h"

#include "Characters/Mob/MobBase.h"
#include "GatherableActor/GatherableActorBase.h"

#include "Components/CapsuleComponent.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Core/Interact/InteractionComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Manager/TeamManager.h"

// sound
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AManny::AManny()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraSpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraSpringArm->bInheritYaw = true;
	CameraSpringArm->bInheritPitch = true;
	CameraSpringArm->bInheritRoll = true;

	// Create a follow camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create AIPerceptionStimuliSourceComponent
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSourceComponent"));

	// Create Team Component and Setting
	TeamComponent = CreateDefaultSubobject<UTeamComponent>(TEXT("TeamComponent"));
	TeamComponent->SetTeamType(ETeamType::Player);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	ViewModeComponent = CreateDefaultSubobject<UViewModeComponent>(TEXT("ViewModeComponent"));

	AttackComponent = CreateDefaultSubobject<UPlayerAttackComponent>(TEXT("AttackComponent"));

	StatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("StatComponent"));

	GatherComponent = CreateDefaultSubobject<UGatherComponent>(TEXT("GatherComponent"));
}

// Called when the game starts or when spawned
void AManny::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = StatComponent->BaseSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BattleContext, 0);
			Subsystem->AddMappingContext(CoreContext, 1);
		}
	}

	AnimInstance = GetMesh()->GetAnimInstance();
}

// Called to bind functionality to input
void AManny::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AManny::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AManny::Look);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AManny::Interact);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AManny::LeftClickAction);
	}
}

FGenericTeamId AManny::GetGenericTeamId() const
{
	return TeamComponent->GetGenericTeamId();
}

void AManny::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamComponent->SetTeamType(static_cast<ETeamType>(NewTeamID.GetId()));
}

void AManny::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AManny::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AManny::Interact(const FInputActionValue& Value)
{
	if (UInteractionComponent* InteractionComp = FindComponentByClass<UInteractionComponent>())
	{
		InteractionComp->TryInteract();
	}
}

void AManny::LeftClickAction(const FInputActionValue& Value)
{
	// 1. 라인 트레이스의 시작점과 끝점 계산
	FVector Start, End;
	FRotator ViewRot;
	GetActorEyesViewPoint(Start, ViewRot);
	End = Start + ViewRot.Vector() * StatComponent->AttackableDistance;

	// 2. 충돌 파라미터 설정
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// 3. 실제 라인 트레이스 수행
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Pawn,
		Params
	);
	AActor* HitActor = bHit ? Hit.GetActor() : nullptr;

	// 4. 디버그용으로 시각화	
#if WITH_EDITOR
	// DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Red : FColor::Green, false, 0.1f, 0, 1.0f);
#endif

	auto MobBase = Cast<AMobBase>(HitActor);
	auto GatherableActor = Cast<AGatherableActorBase>(HitActor);

	if (MobBase && AttackComponent)
	{
		AttackComponent->StartAttack();
	}
	else if (GatherableActor && GatherComponent)
	{
		GatherComponent->StartGather();
	}
}

void AManny::ReceiveDamage_Implementation(const float DamageAmount)
{
	if (StatComponent)
	{
		StatComponent->ApplyDamage(DamageAmount);

		if (0.0f <= StatComponent->CurrentHealth)
		{
			if (!AnimInstance->Montage_IsPlaying(Hit1Montage) || !AnimInstance->Montage_IsPlaying(Hit2Montage))
			{
				const int motion = FMath::RandRange(0, 1);
				switch (motion)
				{
				case 0:
					AnimInstance->Montage_Play(Hit1Montage, 1.0f);
					break;
				case 1:
					AnimInstance->Montage_Play(Hit2Montage, 1.0f);
					break;
				default:
					break;
				}
			}
		}
		else
		{
			Die();	// 죽음 처리
		}
	}
}
