#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Character_Monster.generated.h"

UCLASS()
class TEAM10_PROJECT_API ACharacter_Monster : public ACharacter, public IGenericTeamAgentInterface
{
    GENERATED_BODY()

public:
    ACharacter_Monster();
    virtual FGenericTeamId GetGenericTeamId() const override;

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
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
    FGenericTeamId TeamId;

    // 히트박스 겹침 이벤트 함수
    UFUNCTION()
    void OnAttackHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // ** 여기에 추가: 공격 종료 후 이동 재개를 위한 함수 및 타이머 핸들러 선언 **
    void OnAttackEnd();

    FTimerHandle TimerHandle_AttackEnd;

    // 기존에 있던 테스트용 함수가 필요 없으면 지우셔도 됩니다.
    void ForceAttackTest();
};
