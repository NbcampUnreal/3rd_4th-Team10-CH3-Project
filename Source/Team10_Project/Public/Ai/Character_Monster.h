#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Character_Monster.generated.h"

// --- �߰��� �κ� ---
// ������ �������� �˸��� ��ȣ(Delegate)�� �����մϴ�.
DECLARE_DELEGATE(FOnAttackFinished);
// -----------------

UCLASS()
class TEAM10_PROJECT_API ACharacter_Monster : public ACharacter, public IGenericTeamAgentInterface
{
    GENERATED_BODY()

public:
    ACharacter_Monster();
    virtual FGenericTeamId GetGenericTeamId() const override;

    // ���� �Լ�
    UFUNCTION(BlueprintCallable)
    void Attack();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ApplyCustomDamage(int32 DamageAmount);

    // ���� ������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    int32 MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
    int32 Health;

   

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    float MaxStamina;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
    float Stamina;

    void Die();

    // ���� �ִϸ��̼� ��Ÿ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TObjectPtr<class UAnimMontage> AttackAnimMontage;

    // ���� ���� ��Ʈ�ڽ�
    UPROPERTY(VisibleAnywhere, Category = "Attack")
    TObjectPtr<class UBoxComponent> AttackHitbox;

    // --- �߰��� �κ� ---
    // ������ ��ȣ�� ������ �����ϴ�.
    FOnAttackFinished OnAttackFinished;
    // -----------------

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