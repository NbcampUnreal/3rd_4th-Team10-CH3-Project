#include "AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	CurrentStamina = MaxStamina;
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCanStaminaRegen)
	{
		CurrentStamina += DeltaTime * StaminaRegenRate;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);
	}
	
}

void UAttributeComponent::StartSprintStaminaLogic()
{
	bCanStaminaRegen = false;
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
}

void UAttributeComponent::StopSprintStaminaLogic()
{
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &UAttributeComponent::StartStaminaRegen, StaminaRegenTimer, false);
}

void UAttributeComponent::ConsumeStamina(float DeltaTime)
{
	CurrentStamina -= StaminaComsumeRate * DeltaTime;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);
}

bool UAttributeComponent::CanSprint() const
{
	return CurrentStamina >= MinStamina;
}

void UAttributeComponent::StartStaminaRegen()
{
	bCanStaminaRegen = true;
}




