#pragma once

#include "CoreMinimal.h"
#include "AttributeComponent.h"
#include "GameFramework/Character.h"
#include "Curves/CurveFloat.h"
#include "Camera/PlayerCameraManager.h"
<<<<<<< HEAD
#include "Components/TimelineComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"
=======
#include "GenericTeamAgentInterface.h" // <--- [AI 기능 추가] 1. 헤더 추가
>>>>>>> 6f0121d91b923657b5fec199631b3d8a051d944b
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
//                                              👇 [AI 기능 추가] 2. 인터페이스 상속
UCLASS()
class TEAM10_PROJECT_API AMyCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMyCharacter();

	// ----- [AI 기능 추가] 3. 팀 ID 함수 선언 -----
	virtual FGenericTeamId GetGenericTeamId() const override;
	// ------------------------------------------

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Landed(const FHitResult& Hit) override;
<<<<<<< HEAD
	
=======

	// ----- 캐릭터 속성 -----

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float Defence;

	// --------------------

	// ----- 속도 및 기타 속성 -----
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float NormalSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeedMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float ZoomSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float ZoomSpeedMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun")
	float DefaultFOV = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun")
	float ZoomedFOV = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun")
	float ZoomInterpSpeed = 10.f;

	// (캡슐 컴포넌트) 서있는 상태 88, 앉아있는 상태 44
	float StandingCapsuleHalfHeight = 88.f;
	float CrouchingCapsuleHalfHeight = 44.f;

	// --------------------------

>>>>>>> 6f0121d91b923657b5fec199631b3d8a051d944b
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
<<<<<<< HEAD
    void ToggleFlashlight();

	void EquipWeapon();
	void UnEquipWeapon();
	
	// ---------------------------

	// ----- Timeline 함수 -----
	
=======

	// ---------------------------

	void SetCharacterState(ECharacterState NewState);
	void UpdateGroundState();
	float ApplyMovementSpeedByState();

>>>>>>> 6f0121d91b923657b5fec199631b3d8a051d944b
	UFUNCTION()
	void UpdateSprintFOV(float Value);

	UFUNCTION(BlueprintCallable, Category = "Sprinting")
	void StartSprintFOV();

	UFUNCTION(BlueprintCallable, Category = "Sprinting")
	void StopSprintFOV();

	UFUNCTION()
	void UpdateCrouch(float Value);

<<<<<<< HEAD
	// --------------------------
	
	// ----- 캐릭터 상태 관리 -----
	
	FVector2D LastInputVector;
	
=======
>>>>>>> 6f0121d91b923657b5fec199631b3d8a051d944b
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

<<<<<<< HEAD
	// ----- 컴포넌트 -------
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	TObjectPtr<UAttributeComponent> AttributeComponent;
	
=======

	// ----- 컴포넌트 -----

>>>>>>> 6f0121d91b923657b5fec199631b3d8a051d944b
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> Pivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> CharacterArms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> FirstPersonCamera;
<<<<<<< HEAD
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Sounnds")
	TObjectPtr<USceneComponent> Sounds;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Sounnds")
	TObjectPtr<UAudioComponent> FootStepSounds;
	
=======

>>>>>>> 6f0121d91b923657b5fec199631b3d8a051d944b
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

<<<<<<< HEAD
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
	
public:
	// ----- Input 바인딩 -----
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// -----------------------

	// ----- 속성 접근 함수 -----
	
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	UAttributeComponent* GetAttributeComponent() const { return AttributeComponent.Get(); }
	
	// --------------------------

};
=======
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private: // <--- [AI 기능 추가] 4. AI 관련 변수
	FGenericTeamId TeamId;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAIPerceptionStimuliSourceComponent* StimuliSource;
};
>>>>>>> 6f0121d91b923657b5fec199631b3d8a051d944b
