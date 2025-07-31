#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
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

	GetMesh()->SetOwnerNoSee(true);
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterState(ECharacterState::Idle);
	
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller && Pivot)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const float AimPitch = ControlRotation.Pitch;

		Pivot->SetRelativeRotation(FRotator(AimPitch, 0.f, 0.f));
	}

	if (CurrentState == ECharacterState::Idle || CurrentState == ECharacterState::Walking)
	{
		if (FMath::IsNearlyZero(GetVelocity().Size()))
		{
			SetCharacterState(ECharacterState::Idle);
		}
		else
		{
			SetCharacterState(ECharacterState::Walking);
		}
	}
}

void AMyCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UpdateGroundState();
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

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
	Crouch();
	bWantsToSprint = false;
	SetCharacterState(ECharacterState::Crouching);
}

void AMyCharacter::EndCrouch()
{
	UnCrouch();
	UpdateGroundState();
}

void AMyCharacter::ToggleCrouch()
{
	if (bIsCrouched)
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
	if (CurrentState == ECharacterState::Crouching || CurrentState == ECharacterState::Jumping)
	{
		return;
	}
	
	if (GetCharacterMovement())
	{
		if (GetCharacterMovement()->IsFalling() || GetCharacterMovement()->IsCrouching() || bIsZoomed)
		{
			return;
		}

		bWantsToSprint = true;
		SetCharacterState(ECharacterState::Sprinting);
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AMyCharacter::StopSprint()
{
	if (CurrentState == ECharacterState::Crouching || CurrentState == ECharacterState::Jumping)
	{
		return;
	}
	
	if (GetCharacterMovement())
	{
		bWantsToSprint = false;
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		UpdateGroundState();
	}
}

void AMyCharacter::StartJump()
{
	if (CurrentState == ECharacterState::Crouching)
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
	GetCharacterMovement()->MaxWalkSpeed = ZoomSpeed;

}
void AMyCharacter::StopZoom()
{
	bIsZoomed = false;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
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

