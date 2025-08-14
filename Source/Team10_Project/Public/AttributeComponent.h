#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM10_PROJECT_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	
	void StartSprintStaminaLogic();
	
	void StopSprintStaminaLogic();
	
	void ConsumeStamina(float DeltaTime);

	bool CanSprint() const;
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ----- 기본 속성 -----
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float Health;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float Defence = 10; // 현재 미사용

	// --------------------

	// ----- 스태미나 -----

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Stamina")
	float MaxStamina = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Stamina")
	float CurrentStamina;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Stamina")
	float StaminaRegenRate = 20.f; // 스태미나 회복량
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Stamina")
	float StaminaComsumeRate = 30.f; // 스태미나 소모량
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Stamina")
	float StaminaRegenTimer = 3.f; // 스태미나 회복 딜레이
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Stamina")
	float MinStamina = 10.f; // 최소 스태미나

	FTimerHandle StaminaRegenTimerHandle; // 스태미나 회복 딜레이 핸들

	bool bCanStaminaRegen = true;
	
	void StartStaminaRegen();

	// ---------------------

public:

    // ----- Setter 함수 -----

    void SetHealth(float NewHealth);
    void SetDefence(float NewDefence);
    void SetStamina(float NewStamina);
    
    void ModityHealth(float Amount);
    void ModityDefence(float Amount);
    void ModityStamina(float Amount);
    
    // -----------------------
	
	// ----- 속도 및 기타 속성 -----
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float NormalSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 550.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeed = 150.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float ZoomSpeed = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeedMultiplier = 1.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float ZoomSpeedMultiplier = 0.7f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeedMultiplier = 0.5f;

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

	// ----- Getter 함수 (INLINE) -----
	
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetStamina() const { return CurrentStamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }
	FORCEINLINE float GetMinSprintStamina() const { return MinStamina; }

	// -------------------------------
	
};
