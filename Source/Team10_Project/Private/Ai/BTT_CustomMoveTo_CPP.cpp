#include "Ai/BTT_CustomMoveTo_CPP.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h" // AI MoveTo �Լ��� ���� �ʿ�

UBTT_CustomMoveTo_CPP::UBTT_CustomMoveTo_CPP()
{
	NodeName = TEXT("CPP_CustomMoveTo");
}

EBTNodeResult::Type UBTT_CustomMoveTo_CPP::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* ControllingPawn = AIController->GetPawn();
	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	// �����忡�� ��ǥ��(TargetActor) ������ �����ɴϴ�.
	UObject* TargetObject = BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey());
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (TargetActor)
	{
		// '1�� Ű' �׽�Ʈ���� �۵��ߴ� �ٷ� �� �Լ��� ȣ���մϴ�.
		UAIBlueprintHelperLibrary::SimpleMoveToActor(AIController, TargetActor);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}