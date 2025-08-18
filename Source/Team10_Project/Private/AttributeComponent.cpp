#include "AttributeComponent.h"
#include "MyCharacter.h"
#include "Engine/Engine.h" // GEngine->AddOnScreenDebugMessage�� ���� �߰�

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

void UAttributeComponent::SetHealth(float NewHealth)
{
    Health = NewHealth;

    if (FMath::IsNearlyZero(Health))
    {
        AMyCharacter* MyCharacter = Cast<AMyCharacter>(GetOwner());
        if (MyCharacter)
        {
            MyCharacter->OnDeath();
        }
    }
}

void UAttributeComponent::SetDefence(float NewDefence)
{
    Defence = NewDefence;
}

void UAttributeComponent::SetStamina(float NewStamina)
{
    CurrentStamina = NewStamina;
}

void UAttributeComponent::ModityHealth(float Amount)
{
    // --- ���� �߰��� �κ� ���� ---
    // Amount�� ����(-) ������ ���� ���� �ٷ� ������� �޴� ����Դϴ�.
    if (Amount < 0.f)
    {
        // FMath::Abs()�� ����� ������ ����� �ٲ㼭 ���� ���� ����մϴ�.
        const float DamageTaken = FMath::Abs(Amount);

        // ��� �α� â�� ������(Error)���� �α׸� ǥ���մϴ�.
        UE_LOG(LogTemp, Error, TEXT("takedameg: %f"), DamageTaken);

        // ���� ȭ�� ���� ��ܿ� 5�� ���� ���������� ���� ������� ǥ���մϴ�.
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("���� �����: %.2f"), DamageTaken));
        }
    }
    // --- ���� �߰��� �κ� �� ---

    float NewHealth = FMath::Clamp(Health + Amount, 0.f, MaxHealth);
    SetHealth(NewHealth);
}

void UAttributeComponent::ModityDefence(float Amount)
{
    float NewDefence = FMath::Max(0.f, Defence + Amount);
    SetDefence(NewDefence);
}

void UAttributeComponent::ModityStamina(float Amount)
{
    float NewStamina = FMath::Clamp(CurrentStamina + Amount, 0.f, MaxStamina);
    SetStamina(NewStamina);
}