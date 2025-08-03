#pragma once

#include "CoreMinimal.h"
#include "AttributeComponent.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Camera/PlayerCameraManager.h"
#include "MyCharacter.generated.h"

class UCameraComponent;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle     UMETA(DisplayName = "Idle"),
	Walking  UMETA(DisplayName = "Walking"),
	Sprinting UMETA(DisplayName = "Sprinting"),
	Jumping  UMETA(DisplayName = "Jumping")
};
UCLASS()
class TEAM10_PROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Landed(const FHitResult& Hit) override;
	
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
	// ----- Input 바인딩 -----
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// -----------------------

	// ----- 속성 접근 함수 -----
	
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	UAttributeComponent* GetAttributeComponent() const { return AttributeComponent.Get(); }
	
	// --------------------------

};
