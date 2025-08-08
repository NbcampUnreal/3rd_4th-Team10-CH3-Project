#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Camera/PlayerCameraManager.h"
#include "GenericTeamAgentInterface.h" // <--- [AI 기능 추가] 1. 헤더 추가
#include "MyCharacter.generated.h"

class UCameraComponent;
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

	// ---------------------------

	void SetCharacterState(ECharacterState NewState);
	void UpdateGroundState();
	float ApplyMovementSpeedByState();

	UFUNCTION()
	void UpdateSprintFOV(float Value);

	UFUNCTION(BlueprintCallable, Category = "Sprinting")
	void StartSprintFOV();

	UFUNCTION(BlueprintCallable, Category = "Sprinting")
	void StopSprintFOV();

	UFUNCTION()
	void UpdateCrouch(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bEquipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsZoomed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsCrouching;

	bool bWantsToSprint;

	UPROPERTY(VisibleAnywhere, Category = "State")
	ECharacterState CurrentState;

	UFUNCTION(BlueprintCallable, Category = "State")
	ECharacterState GetCurrentState() const;


	// ----- 컴포넌트 -----

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> Pivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> CharacterArms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCurveFloat> SprintFOVCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UTimelineComponent> SprintFOVTimeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCurveFloat> CrouchCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UTimelineComponent> CrouchTimeline;

	// -------------------

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private: // <--- [AI 기능 추가] 4. AI 관련 변수
	FGenericTeamId TeamId;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAIPerceptionStimuliSourceComponent* StimuliSource;
};