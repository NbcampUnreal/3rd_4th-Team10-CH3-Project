#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack.generated.h"

// --- �߰��� �κ� ---
// UBehaviorTreeComponent Ŭ������ �̸� �˷��ݴϴ�. (���� ����)
class UBehaviorTreeComponent;
// -----------------

UCLASS(Blueprintable)
class TEAM10_PROJECT_API UBTT_Attack : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTT_Attack();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    // --- �߰��� �κ� ---
    // ������ ���� ���� ��ȣ�� ���� �Լ��� �����մϴ�.
    UFUNCTION()
    void OnAttackFinished(UBehaviorTreeComponent* OwnerComp);
    // -----------------
};