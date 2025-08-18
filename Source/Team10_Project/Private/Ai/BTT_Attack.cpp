#include "Ai/BTT_Attack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Ai/Character_Monster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_Attack::UBTT_Attack()
{
    NodeName = TEXT("CPP_Attack");
    // --- �߰��� �κ� ---
    // �񵿱�(Latent) �۾��� ���� Tick�� �ʿ� ������ ����մϴ�.
    bNotifyTick = false;
    // -----------------
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    ACharacter_Monster* Monster = Cast<ACharacter_Monster>(AIController->GetPawn());
    if (!Monster)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp && !BlackboardComp->GetValueAsBool(FName("IsInAttackRange")))
    {
        return EBTNodeResult::Failed;
    }

    if (Monster->GetCharacterMovement())
    {
        Monster->GetCharacterMovement()->StopMovementImmediately();
        Monster->GetCharacterMovement()->DisableMovement();
    }
    AIController->StopMovement();

    // --- �߰��� �κ� ���� ---
    // ������ ���� ���� ��ȣ�� ���� �� Ŭ������ OnAttackFinished �Լ��� �����ϵ��� �����ϴ�.
    Monster->OnAttackFinished.BindUFunction(this, FName("OnAttackFinished"), &OwnerComp);
    // -------------------

    // ���� ����
    Monster->Attack();

    // --- ������ �κ� ---
    // "�۾� ����"�� �ƴ� "�۾� ���� ��"�� ��ȯ�ؼ� AI�� ��ٸ����� �մϴ�.
    return EBTNodeResult::InProgress;
}

// --- �߰��� �Լ� ---
// ������ ������ ������ Monster->OnAttackFinished ��ȣ�� ���� �� �Լ��� ȣ��˴ϴ�.
void UBTT_Attack::OnAttackFinished(UBehaviorTreeComponent* OwnerComp)
{
    if (OwnerComp)
    {
        // "���� ���� �۾��� ������ �������ϴ�"��� �����̺�� Ʈ���� �˷��ݴϴ�.
        FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
    }
}
// -----------------