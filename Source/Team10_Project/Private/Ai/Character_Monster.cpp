#include "Ai/Character_Monster.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h" // <--- [���� ���� �߰�] �����带 �б� ���� �ʿ�

ACharacter_Monster::ACharacter_Monster()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// --- [AI �� ���� �߰�] ---
	// ������ �� ID�� 1������ �����մϴ�.
	TeamId = FGenericTeamId(1);
	// --------------------

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

// Tick �Լ��� ã�� �� ��� ���� �߰��߽��ϴ�.
// ���� ���� Tick �Լ��� �ִٸ� �� ������ �� �ڵ�� ��ü���ּ���.
void ACharacter_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// --- [���� ���� �߰�] ---
	// �� ĳ���͸� �����ϴ� AI ��Ʈ�ѷ��� �����ɴϴ�.
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		// ��Ʈ�ѷ��� �����带 �����ɴϴ�.
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if (BlackboardComp)
		{
			// �����忡�� ��ǥ��(TargetActor) ������ �����ɴϴ�.
			UObject* TargetObject = BlackboardComp->GetValueAsObject(FName("TargetActor"));
			AActor* TargetActor = Cast<AActor>(TargetObject);

			if (TargetActor)
			{
				// ��ǥ���� �ִٸ�, �� �������� ���� �����Դϴ�.
				FVector Direction = TargetActor->GetActorLocation() - GetActorLocation();
				Direction.Z = 0; // ���Ʒ��� �������� �ʵ��� Z���� 0���� ����
				Direction.Normalize();
				AddMovementInput(Direction, 1.0f);
			}
		}
	}
	// --------------------
}


void ACharacter_Monster::Attack()
{
	if (AttackAnimMontage)
	{
		PlayAnimMontage(AttackAnimMontage);
	}
}

float ACharacter_Monster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCauser && DamageCauser->GetInstigatorController() && DamageCauser->GetInstigatorController()->IsA<APlayerController>())
	{
		// ���� ������ ��� (������ - ����)
		const float DamageToApply = FMath::Max(0.f, DamageAmount - Defence);
		const float ActualDamage = Super::TakeDamage(DamageToApply, DamageEvent, EventInstigator, DamageCauser);

		if (ActualDamage > 0.f)
		{
			Health -= ActualDamage;
			UE_LOG(LogTemp, Warning, TEXT("�÷��̾�� %f�� �������� �޾� ���� ü��: %f"), ActualDamage, Health);

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
	UE_LOG(LogTemp, Warning, TEXT("���Ͱ� �׾����ϴ�."));

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
			UE_LOG(LogTemp, Warning, TEXT("%s ���� 10�� �������� �������ϴ�!"), *PlayerCharacter->GetName());
			AttackHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

FGenericTeamId ACharacter_Monster::GetGenericTeamId() const
{
	return TeamId;
}