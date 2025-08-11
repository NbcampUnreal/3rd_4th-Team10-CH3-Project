#pragma once

#include "CoreMinimal.h"
#include "AttributeComponent.h"
#include "GameFramework/Character.h"
#include "Curves/CurveFloat.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/TimelineComponent.h"
#include "Components/SpotLightComponent.h"
#include "Weapons/Actors/WeaponBase.h"
#include "Weapons/WeaponData.h"
#include "Items/ItemTypes.h"
#include "Kismet/GameplayStatics.h"
#include "GenericTeamAgentInterface.h" // <--- [AI 기능 추가] 1. 헤더 추가
#include "MyCharacter.generated.h"

class UCameraComponent;
class UAudioComponent;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle	UMETA(DisplayName = "Idle"),
	Walking UMETA(DisplayName = "Walking"),
	Sprinting UMETA(DisplayName = "Sprinting"),
	Jumping UMETA(DisplayName = "Jumping")
};
UCLASS()
class TEAM10_PROJECT_API AMyCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMyCharacter();
	
	// ----- 속성 접근 함수 -----
	
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	UAttributeComponent* GetAttributeComponent() const { return AttributeComponent.Get(); }
	
	// --------------------------

	// ----- [AI 기능 추가] 3. 팀 ID 함수 선언 -----
	
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	// ------------------------------------------

    // ----- Getter 함수 -----

    FName GetWeaponSocketName() const { return WeaponSocketName; }

    UFUNCTION(BlueprintPure, Category = "Mesh")
    USkeletalMeshComponent* GetCharacterArms() const { return CharacterArms.Get(); }

    UFUNCTION(BlueprintPure, Category = "Weapon")
    AWeaponBase* GetCurrentWeapon() const { return CurrentWeapon.Get(); }

    // -----------------------

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // ----- 무기 -----

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<AWeaponBase> CurrentWeapon;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponSocketName = TEXT("WeaponSocket");

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    TMap<EWeaponDataType, FWeaponData> WeaponInventory;
    
    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TObjectPtr<UDataTable> WeaponDataTable;

    // -----------------

	// ----- 동작 바인딩 함수 -----

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartCrouch();
	void EndCrouch();
	void ToggleCrouch();
	void StartSprint();
	void StopSprint();
	void Shoot();

	void StartZoom();
	void StopZoom();
	void StartJump();
	void StopJump();
	void Reload();
	void FinishReload();
    void ToggleFlashlight();
    
    void EquipPistol();
    void EquipRifle();
    void EquipShotgun();

	void EquipWeapon(EWeaponDataType WeaponToEquip);
	void UnEquipWeapon();
    void FinishUnEquip();
	
	// ---------------------------

	// ----- Timeline 함수 -----
	
	UFUNCTION()
	void UpdateSprintFOV(float Value);

	UFUNCTION(BlueprintCallable, Category = "Sprinting")
	void StartSprintFOV();

	UFUNCTION(BlueprintCallable, Category = "Sprinting")
	void StopSprintFOV();

	UFUNCTION()
	void UpdateCrouch(float Value);

	// --------------------------
	
	// ----- 캐릭터 상태 관리 -----
	
	FVector2D LastInputVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bEquipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsZoomed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsCloseToWall;
	
	bool bWantsToSprint;

	void SetCharacterState(ECharacterState NewState);
	
	void UpdateGroundState();
	
	void ApplyMovementSpeedByState();
	
	UPROPERTY(VisibleAnywhere, Category = "State")
	ECharacterState CurrentState;

	UFUNCTION(BlueprintCallable, Category = "State")
	ECharacterState GetCurrentState() const;
	
	// ---------------------

	// ----- 컴포넌트 -------
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	TObjectPtr<UAttributeComponent> AttributeComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> Pivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> CharacterArms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> FirstPersonCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Sounnds")
	TObjectPtr<USceneComponent> Sounds;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Sounnds")
	TObjectPtr<UAudioComponent> FootStepSounds;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCurveFloat> SprintFOVCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UTimelineComponent> SprintFOVTimeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCurveFloat> CrouchCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UTimelineComponent> CrouchTimeline;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Flashlight")
	TObjectPtr<USpotLightComponent> Flashlight;

	// -------------------

	// ----- 애니메이션 몽타주 -----
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> ReloadMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> FireMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> EquipMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> UnEquipMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> HolsterMontage;
	
	// ---------------------------

	// ----- 사운드 -----
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Sound")
	TObjectPtr<USoundBase> CrouchingSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Sound")
	TObjectPtr<USoundBase> UnCrouchingSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Sound")
	TObjectPtr<USoundBase> FlashlightOnSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Sound")
	TObjectPtr<USoundBase> FlashlightOffSound;
	
	// -----------------
	
private: // <--- [AI 기능 추가] 4. AI 관련 변수
	FGenericTeamId TeamId;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAIPerceptionStimuliSourceComponent* StimuliSource;
};
