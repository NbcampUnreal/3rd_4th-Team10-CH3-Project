#include "Ai/BTT_Attack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Ai/Character_Monster.h"
#include "GameFramework/CharacterMovementComponent.h" // �� ���� �߰����ּ���.

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

	// --- �׽�Ʈ�� ���� ���� ���� �ڵ带 ��� ��Ȱ��ȭ �մϴ� ---
	// Monster->Attack(); 

	// --- ���, ������ ������ �� ���� �����̴� �ڵ带 �߰��մϴ� ---
	if (Monster->GetCharacterMovement())
	{
		Monster->AddMovementInput(Monster->GetActorForwardVector(), 1.0f);

		// �׽�Ʈ�� �α�: �� ����� ����Ǿ����� Ȯ���մϴ�.
		UE_LOG(LogTemp, Warning, TEXT("BTT_Attack: move to up"));
	}
	// -----------------------------------------------------------

	return EBTNodeResult::Succeeded;
}

// ���� ���� �� �߰�

