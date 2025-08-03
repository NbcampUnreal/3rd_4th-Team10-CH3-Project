#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "ParticleHelper.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyCharacter::AMyCharacter()
{
	Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
	Pivot->SetupAttachment(GetCapsuleComponent());

	CharacterArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterArms"));
	CharacterArms->SetupAttachment(Pivot);

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(CharacterArms, TEXT("head"));
	FirstPersonCamera->bUsePawnControlRotation = true;

	CharacterArms->SetOnlyOwnerSee(true);
	CharacterArms->bCastCapsuleDirectShadow = false;
	CharacterArms->CastShadow = false;

	SprintFOVTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SprintFOVTimeline"));
	CrouchTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CrouchTimeline"));

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));

	GetMesh()->SetOwnerNoSee(true);
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterState(ECharacterState::Idle);

	if (SprintFOVCurve && SprintFOVTimeline)
	{
		FOnTimelineFloat FOVInterpFunction;
		FOVInterpFunction.BindUFunction(this, FName("UpdateSprintFOV"));
		SprintFOVTimeline->AddInterpFloat(SprintFOVCurve, FOVInterpFunction, FName("FOV"));
	}

	if (CrouchCurve && CrouchTimeline)
	{
		FOnTimelineFloat CrouchInterpFunction;
		CrouchInterpFunction.BindUFunction(this, FName("UpdateCrouch"));
        CrouchTimeline->AddInterpFloat(CrouchCurve, CrouchInterpFunction);
	}
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ----- 회전 로직 -----
	
	if (Controller && Pivot)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const float AimPitch = ControlRotation.Pitch;

		Pivot->SetRelativeRotation(FRotator(AimPitch, 0.f, 0.f));
	}

	// ---------------------
	
	// ----- 스태미나 로직 -----
	
	if (CurrentState == ECharacterState::Sprinting)
	{
		AttributeComponent->ConsumeStamina(DeltaTime);

		if (AttributeComponent->GetStamina() <= 0.f || LastInputVector.X <= 0.f)
		{
			StopSprint();
		}
	}

	// -------------------------

	// ----- 캐릭터 상태 로직 -----
	
	if (CurrentState == ECharacterState::Idle || CurrentState == ECharacterState::Walking)
	{
		if (FMath::IsNearlyZero(GetVelocity().Size()))
		{
			SetCharacterState(ECharacterState::Idle);
		}
		else
		{
			StopSprintFOV();
			SetCharacterState(ECharacterState::Walking);
		}
	}

	// -------------------------
}

void AMyCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UpdateGroundState();
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	LastInputVector = MovementVector;

	if (Controller)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.X);
		AddMovementInput(GetActorRightVector(), MovementVector.Y);
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LooAxis = Value.Get<FVector2D>();


	if (Controller)
	{
		AddControllerYawInput(LooAxis.X);
		AddControllerPitchInput(LooAxis.Y);
	}
}

void AMyCharacter::StartCrouch()
{
	StopSprint();
	
	if (CrouchTimeline)
	{
		CrouchTimeline->PlayFromStart();
	}
	bWantsToSprint = false;
	bIsCrouching = true;
	ApplyMovementSpeedByState();
}

void AMyCharacter::EndCrouch()
{
	if (CrouchTimeline)
	{
		CrouchTimeline->Reverse();
	}
	bIsCrouching = false;
	ApplyMovementSpeedByState();
}

void AMyCharacter::UpdateCrouch(float Value)
{
	const float NewHalfHeight = FMath::Lerp(
		AttributeComponent->StandingCapsuleHalfHeight,
		AttributeComponent->CrouchingCapsuleHalfHeight,
		Value);
	GetCapsuleComponent()->SetCapsuleHalfHeight(NewHalfHeight);
}

void AMyCharacter::ToggleCrouch()
{
	if (bIsCrouching)
	{
		EndCrouch();
	}
	else
	{
		StartCrouch();
	}
}

void AMyCharacter::StartSprint()
{
	if (!AttributeComponent->CanSprint() || LastInputVector.X <= 0.f)
	{
		return;
	}

	bWantsToSprint = true;
	
	if (CurrentState == ECharacterState::Jumping || bIsZoomed || bIsCrouching)
	{
		return;
	}
	
	if (GetCharacterMovement())
	{
		if (GetCharacterMovement()->IsFalling())
		{
			return;
		}

		AttributeComponent->StartSprintStaminaLogic();
		
		SetCharacterState(ECharacterState::Sprinting);
		SprintFOVTimeline->PlayFromStart();
	}
}

void AMyCharacter::StopSprint()
{
	bWantsToSprint = false;
	if (CurrentState != ECharacterState::Sprinting)
	{
		return;
	}
	AttributeComponent->StopSprintStaminaLogic();
	
	SprintFOVTimeline->Reverse();
	UpdateGroundState();
}

void AMyCharacter::StartJump()
{
	if (bIsCrouching)
	{
		return;
	}
	Jump();
	SetCharacterState(ECharacterState::Jumping);
}

void AMyCharacter::StopJump()
{
	StopJumping();
}

void AMyCharacter::Shoot()
{
	if (CurrentState == ECharacterState::Sprinting || bIsReloading)
	{
		return;
	}

}

void AMyCharacter::StartZoom()
{
	if (CurrentState == ECharacterState::Sprinting || bIsReloading)
	{
		return;
	}
	bIsZoomed = true;
	ApplyMovementSpeedByState();
}

void AMyCharacter::StopZoom()
{
	bIsZoomed = false;
	ApplyMovementSpeedByState();
}

void AMyCharacter::Reload()
{
	if (CurrentState == ECharacterState::Sprinting || bIsReloading || !bEquipped)
	{
		return;
	}

	StopZoom();
	
	bIsReloading = true;
	UE_LOG(LogTemp, Log, TEXT("Reloading..."));
	
	FTimerHandle ReloadTimer;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AMyCharacter::FinishReload, 2.0f, false);
}

void AMyCharacter::FinishReload()
{
	bIsReloading = false;
	UE_LOG(LogTemp, Log, TEXT("Reload Finished"));
}

void AMyCharacter::SetCharacterState(ECharacterState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	CurrentState = NewState;
	ApplyMovementSpeedByState();
	UE_LOG(LogTemp, Warning, TEXT("Character State Changed: %s"), *UEnum::GetValueAsString(CurrentState));
}

void AMyCharacter::UpdateGroundState()
{
	if (bWantsToSprint)
	{
		SetCharacterState(ECharacterState::Sprinting);
		return;
	}
	
	if (FMath::IsNearlyZero(GetVelocity().Size()))
	{
		SetCharacterState(ECharacterState::Idle);
	}
	else
	{
		SetCharacterState(ECharacterState::Walking);
	}
}

void AMyCharacter::ApplyMovementSpeedByState()
{
	float BaseSpeed = AttributeComponent->NormalSpeed;
	
	switch (CurrentState)
	{
	case ECharacterState::Sprinting:
		BaseSpeed = AttributeComponent->SprintSpeed;
		break;
	case ECharacterState::Walking:
	case ECharacterState::Idle:
		BaseSpeed = AttributeComponent->NormalSpeed;
		break;
	}

	if (bIsZoomed)
	{
		BaseSpeed *= AttributeComponent->ZoomSpeedMultiplier;
	}
	if (bIsCrouching)
	{
		BaseSpeed *= AttributeComponent->CrouchSpeedMultiplier;
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;	
	}
}

void AMyCharacter::UpdateSprintFOV(float Value)
{
	if (FirstPersonCamera)
	{
		FirstPersonCamera->FieldOfView = Value;	
	}
}

void AMyCharacter::StartSprintFOV()
{
	if (SprintFOVTimeline)
	{
		SprintFOVTimeline->PlayFromStart();
	}
}

void AMyCharacter::StopSprintFOV()
{
	if (SprintFOVTimeline)
	{
		SprintFOVTimeline->Reverse();
	}
}

ECharacterState AMyCharacter::GetCurrentState() const
{
	return CurrentState;
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			EnhancedInputComponent->BindAction(
				PlayerController->MoveAction, 
				ETriggerEvent::Triggered, 
				this, 
				&AMyCharacter::Move
				);
            
			EnhancedInputComponent->BindAction(
				PlayerController->LookAction, 
				ETriggerEvent::Triggered, 
				this, 
				&AMyCharacter::Look
				);
		
			EnhancedInputComponent->BindAction(
				PlayerController->SprintAction, 
				ETriggerEvent::Started, 
				this, 
				&AMyCharacter::StartSprint
				);
			
			EnhancedInputComponent->BindAction(
				PlayerController->SprintAction, 
				ETriggerEvent::Completed, 
				this, 
				&AMyCharacter::StopSprint
				);
			
			EnhancedInputComponent->BindAction(
				PlayerController->CrouchAction, 
				ETriggerEvent::Started, 
				this, 
				&AMyCharacter::StartCrouch
				);
			
			EnhancedInputComponent->BindAction(
				PlayerController->CrouchAction, 
				ETriggerEvent::Completed, 
				this, 
				&AMyCharacter::EndCrouch
				);

			EnhancedInputComponent->BindAction(
				PlayerController->ToggleCrouchAction,  
				ETriggerEvent::Started, 
				this, 
				&AMyCharacter::ToggleCrouch
				);
			
			EnhancedInputComponent->BindAction(
				PlayerController->JumpAction,
				ETriggerEvent::Started,
				this,
				&AMyCharacter::StartJump
				);
			
			EnhancedInputComponent->BindAction(
				PlayerController->JumpAction,
				ETriggerEvent::Completed,
				this,
				&AMyCharacter::StopJump
				);
			
			EnhancedInputComponent->BindAction(
				PlayerController->ShootAction,
				ETriggerEvent::Triggered,
				this,
				&AMyCharacter::Shoot
				);
			
			EnhancedInputComponent->BindAction(
				PlayerController->ZoomAction,
				ETriggerEvent::Triggered,
				this,
				&AMyCharacter::StartZoom
				);
			EnhancedInputComponent->BindAction(
				PlayerController->ZoomAction,
				ETriggerEvent::Completed,
				this,
				&AMyCharacter::StopZoom
				);
			
			EnhancedInputComponent->BindAction(
				PlayerController->ReloadAction,
				ETriggerEvent::Triggered,
				this,
				&AMyCharacter::Reload
				);
		}
	}
}

