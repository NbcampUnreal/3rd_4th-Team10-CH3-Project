#include "AttributeComponent.h"
#include "MyCharacter.h"
#include "Engine/Engine.h" // GEngine->AddOnScreenDebugMessage를 위해 추가

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
    // --- 제가 추가한 부분 시작 ---
    // Amount가 음수(-) 값으로 들어올 때가 바로 대미지를 받는 경우입니다.
    if (Amount < 0.f)
    {
        // FMath::Abs()를 사용해 음수를 양수로 바꿔서 보기 좋게 출력합니다.
        const float DamageTaken = FMath::Abs(Amount);

        // 출력 로그 창에 빨간색(Error)으로 로그를 표시합니다.
        UE_LOG(LogTemp, Error, TEXT("takedameg: %f"), DamageTaken);

        // 게임 화면 좌측 상단에 5초 동안 빨간색으로 받은 대미지를 표시합니다.
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("받은 대미지: %.2f"), DamageTaken));
        }
    }
    // --- 제가 추가한 부분 끝 ---

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