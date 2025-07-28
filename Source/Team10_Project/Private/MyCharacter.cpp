#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyCharacter::AMyCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	GetMesh()->bOwnerNoSee = true;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(GetCapsuleComponent());
	CameraComp->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	CameraComp->bUsePawnControlRotation = true;
	
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
}

void AMyCharacter::EndCrouch()
{
	UnCrouch();
}

void AMyCharacter::ToggleCrouch()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}


void AMyCharacter::StartSprint()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AMyCharacter::StopSprint()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void AMyCharacter::Shoot()
{
}

void AMyCharacter::Zoom()
{
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
				&AMyCharacter::Jump
				);
			
			EnhancedInputComponent->BindAction(
				PlayerController->JumpAction,
				ETriggerEvent::Completed,
				this,
				&AMyCharacter::StopJumping
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
				&AMyCharacter::Zoom
				);
		}
	}
}

