#include "Ai/Character_Monster.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h" // <--- ������ ��� ���� �ʿ��մϴ�.

ACharacter_Monster::ACharacter_Monster()
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    // �� ID�� 1������ �����մϴ�.
    TeamId = FGenericTeamId(1);

    // Status ���� �ʱ�ȭ
    MaxHealth = 100.0f;
    Health = MaxHealth;
    Defence = 5.0f;
    MaxStamina = 100.0f;
    Stamina = MaxStamina;

    // ��Ʈ�ڽ� ���� �� ����
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
    UCharacterMovementComponent* MoveComp = GetCharacterMovement();
    if (MoveComp)
    {
        MoveComp->StopMovementImmediately();
        MoveComp->Velocity = FVector::ZeroVector;
        MoveComp->DisableMovement();
    }

    if (AttackAnimMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackAnimMontage))
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

void ACharacter_Monster::OnAttackEnd()
{
    UCharacterMovementComponent* MoveComp = GetCharacterMovement();
    if (MoveComp)
    {
        MoveComp->SetMovementMode(EMovementMode::MOVE_Walking);
    }
}

float ACharacter_Monster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (DamageCauser && DamageCauser->GetInstigatorController() && DamageCauser->GetInstigatorController()->IsA<APlayerController>())
    {
        const float DamageToApply = FMath::Max(0.f, DamageAmount - Defence);
        const float ActualDamage = Super::TakeDamage(DamageToApply, DamageEvent, EventInstigator, DamageCauser);

        if (ActualDamage > 0.f)
        {
            Health -= ActualDamage;
            if (Health <= 0.f)
            {
                Die();
            }
        }
        return ActualDamage;
    }
    return 0.0f;
}

void ACharacter_Monster::Die()
{
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController && AIController->BrainComponent)
    {
        AIController->BrainComponent->StopLogic("Death");
    }
    SetLifeSpan(5.0f);
}

void ACharacter_Monster::OnAttackHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
        if (PlayerCharacter && PlayerCharacter->IsPlayerControlled())
        {
            UGameplayStatics::ApplyDamage(PlayerCharacter, 10.0f, GetController(), this, nullptr);
            AttackHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
}

FGenericTeamId ACharacter_Monster::GetGenericTeamId() const
{
    return TeamId;
}

//
// ? ��ũ ���� �ذ��� ���� �ʼ� �Լ� ����
//
void ACharacter_Monster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // AI ���ʹ� �⺻������ �Է��� ������� ������,
    // ��ũ ���� ������ ���� �� ���Ǵ� �ݵ�� �ʿ��մϴ�.
}
