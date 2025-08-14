#include "MyCharacter.h"
#include "MyPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Weapons/Actors/RangeWeapon.h"

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

    InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
    InteractSphere->SetupAttachment(RootComponent);
    InteractSphere->SetSphereRadius(250.f);;
    InteractSphere->SetCollisionProfileName(TEXT("Trigger"));

	Sounds = CreateDefaultSubobject<USceneComponent>(TEXT("Sounds"));
	Sounds->SetupAttachment(GetCapsuleComponent());

	FootStepSounds = CreateDefaultSubobject<UAudioComponent>(TEXT("FootStepSounds"));
	FootStepSounds->SetupAttachment(Sounds);

	SprintFOVTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SprintFOVTimeline"));
	CrouchTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CrouchTimeline"));
    RecoilTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RecoilTimeline"));

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));

	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetupAttachment(FirstPersonCamera);
	Flashlight->SetVisibility(false);
	Flashlight->Intensity = 5000.f;
	Flashlight->InnerConeAngle = 25.f;
	Flashlight->OuterConeAngle = 30.f;

	GetMesh()->SetOwnerNoSee(true);
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	GetCharacterMovement()->MaxAcceleration = 512;
	GetCharacterMovement()->BrakingDecelerationWalking = 512;

	TeamId = FGenericTeamId(0);

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
	if (StimuliSource)
	{
		StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimuliSource->RegisterWithPerceptionSystem();
	}

}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterState(ECharacterState::Idle);
    CurrentRangeType = ERangeType::None;

	bEquipped = false;
	CharacterArms->SetVisibility(false);

    InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnInteractBeginOverlap);
    InteractSphere->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnInteractEndOverlap);
    
    if (WeaponDataTable)
    {
        TArray<FName> RowNames = WeaponDataTable->GetRowNames();
        for (const FName& RowName : RowNames)
        {
            FWeaponData* Data = WeaponDataTable->FindRow<FWeaponData>(RowName, TEXT(""));
            if (Data)
            {
                ERangeType WeaponType;
                if (RowName == "Pistol") WeaponType = ERangeType::Pistol;
                else if (RowName == "Rifle") WeaponType = ERangeType::Rifle;
                else if (RowName == "Shotgun") WeaponType = ERangeType::Shotgun;
                else continue;

                //WeaponInventory.Add(WeaponType, *Data);
            }
        }
    }

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

    if (RecoilCurve && RecoilTimeline)
    {
        FOnTimelineFloat RecoilInterpFunction;
        RecoilInterpFunction.BindUFunction(this, FName("UpdateRecoil"));
        RecoilTimeline->AddInterpFloat(RecoilCurve, RecoilInterpFunction);
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

		if (AttributeComponent->GetStamina() <= 0.f || LastInputVector.X <= 0.f || bIsCloseToWall)
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

	// ----- 벽 체크 로직 -----
	
	FVector Start = FirstPersonCamera->GetComponentLocation();
	FVector End = Start + (FirstPersonCamera->GetForwardVector() * 100.f);
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	bIsCloseToWall = bHit;
	
	// ----------------------
}

void AMyCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UpdateGroundState();
}

void AMyCharacter::OnInteractBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherActor->Implements<UItemInterface>())
    {
        OverlappingItems.Add(OtherActor);
        UE_LOG(LogTemp, Log, TEXT("[Character]Overlapping Items: %d"), OverlappingItems.Num());
    }
}

void AMyCharacter::OnInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this && OtherActor->Implements<UItemInterface>())
    {
        OverlappingItems.Remove(OtherActor);
        UE_LOG(LogTemp, Log, TEXT("[Character]Overlapping Items: %d"), OverlappingItems.Num());
    }
}

void AMyCharacter::Interact()
{
    if (OverlappingItems.IsEmpty())
    {
        return;
    }

    AActor* InteractingItem = OverlappingItems[0];
    if (InteractingItem && InteractingItem->Implements<UItemInterface>())
    {
        UAnimInstance* AnimInstance = CharacterArms->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(InteractMontage, 1.4f);
        }
        PickupWeapon(ERangeType::Pistol);   // 테스트
        InteractingItem->Destroy();         // 테스트
    }
}

void AMyCharacter::PickupWeapon(ERangeType RangeTypeToPickup)
{
    if (WeaponInventory.Contains(RangeTypeToPickup))
    {
        // To Do: 이미 가지고 있는 RangeType (AmmoAmount만 증가)
        return;
    }

    if (WeaponDataTable)
    {
        const FName RowName = GetWeaponRowNameFromType(RangeTypeToPickup);
        FWeaponData* Data = WeaponDataTable->FindRow<FWeaponData>(RowName, TEXT(""));

        if (Data)
        {
            WeaponInventory.Add(RangeTypeToPickup, *Data);
            UE_LOG(LogTemp, Warning, TEXT("Pickup New Weapon: %s"), *RowName.ToString());

            EquipWeapon(RangeTypeToPickup);
        }
    }
}

void AMyCharacter::OnDeath()
{
    
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
	
	if (CrouchingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CrouchingSound, GetActorLocation());   
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
	
	if (UnCrouchingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, UnCrouchingSound, GetActorLocation());  
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

void AMyCharacter::UpdateRecoil(float Value)
{
    CurrentRecoilPitch = Value;
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
	if (!AttributeComponent->CanSprint() || LastInputVector.X <= 0.f || bIsCloseToWall)
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

bool AMyCharacter::CanShoot()
{
    if (CurrentState == ECharacterState::Sprinting || bIsReloading)
    {
        return false;
    }
    
    ARangeWeapon* RangeWeapon = Cast<ARangeWeapon>(CurrentWeapon);
    if (RangeWeapon)
    {
        if (RangeWeapon->GetLoadedAmmoAmount() <= 0)
        {
            return false;
        }
    }
    return true;   
}
void AMyCharacter::StartShoot()
{
	if (!CanShoot())
	{
		return;
	}

    if (CurrentWeapon)
    {
        CurrentWeapon->StartFire();
        bIsFiring = true;

        if (RecoilTimeline)
        {
            RecoilTimeline->PlayFromStart();
        }
        
        if (FireCameraShakeClass)
        {
            APlayerController* PlayerController = Cast<APlayerController>(GetController());
            if (PlayerController)
            {
                PlayerController->ClientStartCameraShake(FireCameraShakeClass);
            }
        }
    }
}

void AMyCharacter::StopShoot()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        bIsFiring = false;
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
    
    ARangeWeapon* RangeWeapon = Cast<ARangeWeapon>(CurrentWeapon);
    if (RangeWeapon)
    {
        int32 LoadAmmo = RangeWeapon->GetLoadedAmmoAmount();
        int32 MaxAmmo = RangeWeapon->GetMaxAmmoAmount();
        if (MaxAmmo <= LoadAmmo)
        {
            return;
        }
    }
    
    UAnimMontage* MontageToPlay = nullptr;
    const FName WeaponRowName = GetWeaponRowNameFromType(CurrentRangeType);
    
    if (WeaponRowName != NAME_None)
    {
        const FWeaponData* WeaponData = WeaponDataTable->FindRow<FWeaponData>(WeaponRowName, TEXT(""));
        if (WeaponData)
        {
            if (RangeWeapon->GetLoadedAmmoAmount() > 0)
            {
                MontageToPlay = WeaponData->ReloadTacMontage; 
            }
            else
            {
                MontageToPlay = WeaponData->ReloadEmptyMontage;
            }
        }
    }

	UAnimInstance* AnimInstance = CharacterArms->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageToPlay, 1.f);
	}
	
	StopZoom();
	bIsReloading = true;

	FTimerHandle ReloadTimer;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AMyCharacter::FinishReload, 2.0f, false);
}

void AMyCharacter::FinishReload()
{
    ARangeWeapon* RangeWeapon = Cast<ARangeWeapon>(CurrentWeapon);
    if (RangeWeapon)
    {
        RangeWeapon->Reload(this);
    }
	bIsReloading = false;
	UE_LOG(LogTemp, Log, TEXT("Reload Finished"));
}

void AMyCharacter::ToggleFlashlight()
{
	if (Flashlight)
	{
		Flashlight->ToggleVisibility();
		
		if (Flashlight->IsVisible())
		{
			if (FlashlightOnSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FlashlightOnSound, GetActorLocation());
			}
		}
		else
		{
			if (FlashlightOffSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FlashlightOffSound, GetActorLocation());
			}
		}
	}
}

void AMyCharacter::EquipPistol()
{
    if (WeaponInventory.Contains(ERangeType::Pistol))
    {
        EquipWeapon(ERangeType::Pistol);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Pistol is not in inventory!"));
    }
}

void AMyCharacter::EquipRifle()
{
    if (WeaponInventory.Contains(ERangeType::Rifle))
    {
        EquipWeapon(ERangeType::Rifle);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Rifle is not in inventory!"));
    }
}

void AMyCharacter::EquipShotgun()
{
    if (WeaponInventory.Contains(ERangeType::Shotgun))
    {
        EquipWeapon(ERangeType::Shotgun);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Shotgun is not in inventory!"));
    }
}

void AMyCharacter::EquipWeapon(ERangeType WeaponToEquip)
{
    if (CurrentWeapon && CurrentRangeType == WeaponToEquip)
    {
        return;
    }
    
	if (CurrentWeapon)
	{
	    UnEquipWeapon();
	}

    if (WeaponInventory.Contains(WeaponToEquip))
    {
        CurrentRangeType = WeaponToEquip;
        const FWeaponData& NewWeaponData = WeaponInventory[WeaponToEquip];

        CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(NewWeaponData.WeaponClass);
        if (CurrentWeapon)
        {
            //CurrentWeapon->SetCurrentAmmo(NewWeaponData.CurrentAmmo);
            //CurrentWeapon->SetMaxAmmo(NewWeaponData.MaxAmmo);

            CurrentWeapon->SetOwner(this);
            CurrentWeapon->EquipmentWeapon(this);
            
            UAnimInstance* AnimInstance = CharacterArms->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(EquipMontage, 1.f);
            }
            
            bEquipped = true;
            CharacterArms->SetVisibility(true);
            CurrentWeapon->SetActorHiddenInGame(false);
            
        }
    }
}

void AMyCharacter::UnEquipWeapon()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->Destroy();
        CurrentWeapon = nullptr;
    
        bEquipped = false;
        CharacterArms->SetVisibility(false);
    }
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

ERangeType AMyCharacter::GetRangeType() const
{
    return CurrentRangeType;
}
FName AMyCharacter::GetWeaponRowNameFromType(ERangeType WeaponType) const
{
    switch (WeaponType)
    {
    case ERangeType::Pistol:
        return FName("Pistol");
    case ERangeType::Rifle:
        return FName("Rifle");
    case ERangeType::Shotgun:
        return FName("Shotgun");
    default:
        return NAME_None;
    }
}

void AMyCharacter::SetCurrentWeapon(AWeaponBase* NewWeapon)
{
    CurrentWeapon = NewWeapon;
}

void AMyCharacter::SetAmmoAmount(int NewAmmoAmount)
{
    AmmoAmount = NewAmmoAmount;
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
				ETriggerEvent::Started,
				this,
				&AMyCharacter::StartShoot
			);

            EnhancedInputComponent->BindAction(
                PlayerController->ShootAction,
                ETriggerEvent::Completed,
                this,
                &AMyCharacter::StopShoot
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
				ETriggerEvent::Started,
				this,
				&AMyCharacter::Reload
			);
			
			EnhancedInputComponent->BindAction(
				PlayerController->ToggleFlashlightAction,
				ETriggerEvent::Started,
				this,
				&AMyCharacter::ToggleFlashlight
			);
			
			EnhancedInputComponent->BindAction(
				PlayerController->Key1Action,
				ETriggerEvent::Started,
				this,
				&AMyCharacter::EquipPistol
			);
		    
		    EnhancedInputComponent->BindAction(
                PlayerController->Key2Action,
                ETriggerEvent::Started,
                this,
                &AMyCharacter::EquipRifle
            );
		    
		    EnhancedInputComponent->BindAction(
                PlayerController->Key3Action,
                ETriggerEvent::Started,
                this,
                &AMyCharacter::EquipShotgun
            );
		    
			EnhancedInputComponent->BindAction(
				PlayerController->KeyQAction,
				ETriggerEvent::Started,
				this,
				&AMyCharacter::UnEquipWeapon
			);
		    
		    EnhancedInputComponent->BindAction(
                PlayerController->InteractAction,
                ETriggerEvent::Started,
                this,
                &AMyCharacter::Interact
            );
		    
		}
	}
}

FGenericTeamId AMyCharacter::GetGenericTeamId() const
{
	return TeamId;
}
