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

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ApplyCustomDamage(int32 DamageAmount);

    // 상태 변수들
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    int32 MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
    int32 Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    int32 Defence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    float MaxStamina;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
    float Stamina;


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

    UFUNCTION()
    void OnAttackHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void OnAttackEnd();

    FTimerHandle TimerHandle_AttackEnd;
};