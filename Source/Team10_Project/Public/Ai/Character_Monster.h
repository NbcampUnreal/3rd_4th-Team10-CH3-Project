#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h" // <--- [AI 팀 설정 추가]
#include "Character_Monster.generated.h"

//                                               ?? [AI 팀 설정 추가]
UCLASS()
class TEAM10_PROJECT_API ACharacter_Monster : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACharacter_Monster();

	// --- [AI 팀 설정 추가] ---
	virtual FGenericTeamId GetGenericTeamId() const override;
	// --------------------

	// 공격 함수
	UFUNCTION(BlueprintCallable)
	void Attack();

	// 데미지를 받는 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// --- 상태(Status) 변수들 ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Defence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	float Stamina;


	// 죽었을 때 처리할 함수
	void Die();

	// 공격 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> AttackAnimMontage;

	// 공격 판정 히트박스
	UPROPERTY(VisibleAnywhere, Category = "Attack")
	TObjectPtr<class UBoxComponent> AttackHitbox;

protected:
	virtual void BeginPlay() override;

	// --- [최종 진단 추가] ---
	// Tick 함수 선언을 추가합니다.
	virtual void Tick(float DeltaTime) override;
	// --------------------

private:
	// --- [AI 팀 설정 추가] ---
	FGenericTeamId TeamId;
	// --------------------

	// 히트박스 겹침 이벤트 함수
	UFUNCTION()
	void OnAttackHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};