#include "Ai/BTT_Attack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Ai/Character_Monster.h"
#include "GameFramework/CharacterMovementComponent.h" // 움직임 제어를 위해 필요
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

    // 블랙보드에서 공격 범위 안에 있는지 확인
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp && !BlackboardComp->GetValueAsBool(FName("IsInAttackRange")))
    {
        return EBTNodeResult::Failed;
    }

    // --- 이동 멈추기 (보강된 부분) ---
    if (Monster->GetCharacterMovement())
    {
        Monster->GetCharacterMovement()->StopMovementImmediately();
        Monster->GetCharacterMovement()->DisableMovement();
    }

    AIController->StopMovement(); // ← 이 부분 추가
    // ---------------------------------

    // 공격 실행
    Monster->Attack();

    return EBTNodeResult::Succeeded;
}
