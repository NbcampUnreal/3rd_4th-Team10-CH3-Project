#include "Ai/BTT_Attack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Ai/Character_Monster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_Attack::UBTT_Attack()
{
    NodeName = TEXT("CPP_Attack");
    // --- 추가된 부분 ---
    // 비동기(Latent) 작업을 위해 Tick이 필요 없음을 명시합니다.
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

    // --- 추가된 부분 시작 ---
    // 몬스터의 공격 종료 신호가 오면 이 클래스의 OnAttackFinished 함수를 실행하도록 묶습니다.
    Monster->OnAttackFinished.BindUFunction(this, FName("OnAttackFinished"), &OwnerComp);
    // -------------------

    // 공격 실행
    Monster->Attack();

    // --- 수정된 부분 ---
    // "작업 성공"이 아닌 "작업 진행 중"을 반환해서 AI가 기다리도록 합니다.
    return EBTNodeResult::InProgress;
}

// --- 추가된 함수 ---
// 몬스터의 공격이 끝나면 Monster->OnAttackFinished 신호에 의해 이 함수가 호출됩니다.
void UBTT_Attack::OnAttackFinished(UBehaviorTreeComponent* OwnerComp)
{
    if (OwnerComp)
    {
        // "이제 공격 작업이 완전히 끝났습니다"라고 비헤이비어 트리에 알려줍니다.
        FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
    }
}
// -----------------