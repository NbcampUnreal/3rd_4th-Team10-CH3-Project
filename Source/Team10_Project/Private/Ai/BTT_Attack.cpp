#include "Ai/BTT_Attack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Ai/Character_Monster.h"
#include "GameFramework/CharacterMovementComponent.h" // ������ ��� ���� �ʿ�
#include "BehaviorTree/BlackboardComponent.h"

UBTT_Attack::UBTT_Attack()
{
    NodeName = TEXT("CPP_Attack");
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

    // �����忡�� ���� ���� �ȿ� �ִ��� Ȯ��
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp && !BlackboardComp->GetValueAsBool(FName("IsInAttackRange")))
    {
        return EBTNodeResult::Failed;
    }

    // --- �̵� ���߱� (������ �κ�) ---
    if (Monster->GetCharacterMovement())
    {
        Monster->GetCharacterMovement()->StopMovementImmediately();
        Monster->GetCharacterMovement()->DisableMovement();
    }

    AIController->StopMovement(); // �� �� �κ� �߰�
    // ---------------------------------

    // ���� ����
    Monster->Attack();

    return EBTNodeResult::Succeeded;
}
