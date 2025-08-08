#include "Ai/BTT_Attack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Ai/Character_Monster.h"
#include "GameFramework/CharacterMovementComponent.h" // 이 줄을 추가해주세요.

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

	// --- 테스트를 위해 기존 공격 코드를 잠시 비활성화 합니다 ---
	// Monster->Attack(); 

	// --- 대신, 강제로 앞으로 한 걸음 움직이는 코드를 추가합니다 ---
	if (Monster->GetCharacterMovement())
	{
		Monster->AddMovementInput(Monster->GetActorForwardVector(), 1.0f);

		// 테스트용 로그: 이 명령이 실행되었는지 확인합니다.
		UE_LOG(LogTemp, Warning, TEXT("BTT_Attack: move to up"));
	}
	// -----------------------------------------------------------

	return EBTNodeResult::Succeeded;
}

// 파일 끝에 줄 추가

