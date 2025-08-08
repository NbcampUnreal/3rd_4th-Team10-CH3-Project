#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h" // <--- [AI �� ���� �߰�]
#include "Character_Monster.generated.h"

//                                               ?? [AI �� ���� �߰�]
UCLASS()
class TEAM10_PROJECT_API ACharacter_Monster : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACharacter_Monster();

	// --- [AI �� ���� �߰�] ---
	virtual FGenericTeamId GetGenericTeamId() const override;
	// --------------------

	// ���� �Լ�
	UFUNCTION(BlueprintCallable)
	void Attack();

	// �������� �޴� �Լ�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// --- ����(Status) ������ ---

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


	// �׾��� �� ó���� �Լ�
	void Die();

	// ���� �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> AttackAnimMontage;

	// ���� ���� ��Ʈ�ڽ�
	UPROPERTY(VisibleAnywhere, Category = "Attack")
	TObjectPtr<class UBoxComponent> AttackHitbox;

protected:
	virtual void BeginPlay() override;

	// --- [���� ���� �߰�] ---
	// Tick �Լ� ������ �߰��մϴ�.
	virtual void Tick(float DeltaTime) override;
	// --------------------

private:
	// --- [AI �� ���� �߰�] ---
	FGenericTeamId TeamId;
	// --------------------

	// ��Ʈ�ڽ� ��ħ �̺�Ʈ �Լ�
	UFUNCTION()
	void OnAttackHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};