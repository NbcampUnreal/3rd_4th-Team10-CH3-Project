#include "Ai/Character_Monster.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h" // <--- [최종 진단 추가] 블랙보드를 읽기 위해 필요

ACharacter_Monster::ACharacter_Monster()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// --- [AI 팀 설정 추가] ---
	// 몬스터의 팀 ID를 1번으로 설정합니다.
	TeamId = FGenericTeamId(1);
	// --------------------

	// Status 변수 초기화
	MaxHealth = 100.0f;
	Health = MaxHealth;
	Defence = 5.0f;
	MaxStamina = 100.0f;
	Stamina = MaxStamina;

	// 히트박스 생성 및 설정
	AttackHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitbox"));
	AttackHitbox->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	AttackHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacter_Monster::BeginPlay()
{
	Super::BeginPlay();
	AttackHitbox->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Monster::OnAttackHitboxOverlapBegin);
}

// Tick 함수를 찾을 수 없어서 새로 추가했습니다.
// 만약 기존 Tick 함수가 있다면 그 내용을 이 코드로 교체해주세요.
void ACharacter_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// --- [최종 진단 추가] ---
	// 이 캐릭터를 조종하는 AI 컨트롤러를 가져옵니다.
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		// 컨트롤러의 블랙보드를 가져옵니다.
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if (BlackboardComp)
		{
			// 블랙보드에서 목표물(TargetActor) 정보를 가져옵니다.
			UObject* TargetObject = BlackboardComp->GetValueAsObject(FName("TargetActor"));
			AActor* TargetActor = Cast<AActor>(TargetObject);

			if (TargetActor)
			{
				// 목표물이 있다면, 그 방향으로 직접 움직입니다.
				FVector Direction = TargetActor->GetActorLocation() - GetActorLocation();
				Direction.Z = 0; // 위아래로 움직이지 않도록 Z축을 0으로 고정
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
		// 실제 데미지 계산 (데미지 - 방어력)
		const float DamageToApply = FMath::Max(0.f, DamageAmount - Defence);
		const float ActualDamage = Super::TakeDamage(DamageToApply, DamageEvent, EventInstigator, DamageCauser);

		if (ActualDamage > 0.f)
		{
			Health -= ActualDamage;
			UE_LOG(LogTemp, Warning, TEXT("플레이어에게 %f의 데미지를 받아 현재 체력: %f"), ActualDamage, Health);

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
	UE_LOG(LogTemp, Warning, TEXT("몬스터가 죽었습니다."));

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
			UE_LOG(LogTemp, Warning, TEXT("%s 에게 10의 데미지를 입혔습니다!"), *PlayerCharacter->GetName());
			AttackHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

FGenericTeamId ACharacter_Monster::GetGenericTeamId() const
{
	return TeamId;
}