#include "Ai/Character_Monster.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

// --- ��� ���� ---
#include "MyCharacter.h"
#include "AttributeComponent.h"
// ----------------

ACharacter_Monster::ACharacter_Monster()
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    TeamId = FGenericTeamId(1);
    MaxHealth = 100;
    Health = MaxHealth;
    MaxStamina = 100.0f;
    Stamina = MaxStamina;
    AttackHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitbox"));
    AttackHitbox->SetupAttachment(GetMesh(), FName("WeaponSocket"));
    AttackHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacter_Monster::BeginPlay()
{
    Super::BeginPlay();
    AttackHitbox->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Monster::OnAttackHitboxOverlapBegin);
}

void ACharacter_Monster::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACharacter_Monster::Attack()
{
    if (auto* MoveComp = GetCharacterMovement())
    {
        MoveComp->StopMovementImmediately();
        MoveComp->Velocity = FVector::ZeroVector;
        MoveComp->DisableMovement();
    }

    AttackHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    if (AttackAnimMontage)
    {
        if (auto* AnimInstance = GetMesh()->GetAnimInstance())
        {
            if (!AnimInstance->Montage_IsPlaying(AttackAnimMontage))
            {
                AnimInstance->Montage_Play(AttackAnimMontage, 1.0f);
                float Duration = AttackAnimMontage->GetPlayLength();
                GetWorld()->GetTimerManager().SetTimer(
                    TimerHandle_AttackEnd,
                    this,
                    &ACharacter_Monster::OnAttackEnd,
                    Duration,
                    false
                );
            }
        }
    }
}

void ACharacter_Monster::OnAttackEnd()
{
    AttackHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (auto* MoveComp = GetCharacterMovement())
    {
        MoveComp->SetMovementMode(EMovementMode::MOVE_Walking);
    }

    
    OnAttackFinished.ExecuteIfBound();
   
}

void ACharacter_Monster::ApplyCustomDamage(int32 DamageAmount)
{
    const float DamageVariationFactor = FMath::FRandRange(0.9f, 1.1f);
    const int32 VariedDamage = FMath::RoundToInt(DamageAmount * DamageVariationFactor);
    const int32 FinalDamage = FMath::Max(0, VariedDamage);

    if (FinalDamage <= 0 || Health <= 0)
    {
        return;
    }

    Health -= FinalDamage;

    if (Health <= 0)
    {
        Die();
    }
}

void ACharacter_Monster::Die()
{
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (auto* AIController = Cast<AAIController>(GetController()))
    {
        if (AIController->BrainComponent)
        {
            AIController->BrainComponent->StopLogic("Death");
        }
    }
    SetLifeSpan(0.1f);
}

void ACharacter_Monster::OnAttackHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        AMyCharacter* MyPlayer = Cast<AMyCharacter>(OtherActor);
        if (MyPlayer)
        {
            UAttributeComponent* PlayerAttributes = MyPlayer->FindComponentByClass<UAttributeComponent>();
            if (PlayerAttributes)
            {
                float BaseAttack = FMath::FRandRange(19.6f, 25.3f);
                float DefenceFactor = FMath::FRandRange(0.8f, 1.2f);
                float EffectiveDefence = PlayerAttributes->GetDefence() * DefenceFactor;
                float MonsterAttackDamage = FMath::Max(0.0f, BaseAttack - EffectiveDefence);

                PlayerAttributes->ModityHealth(MonsterAttackDamage);
            }
            AttackHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
}

FGenericTeamId ACharacter_Monster::GetGenericTeamId() const
{
    return TeamId;
}

void ACharacter_Monster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}