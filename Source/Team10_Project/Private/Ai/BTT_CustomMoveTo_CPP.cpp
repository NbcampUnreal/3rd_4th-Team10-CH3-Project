#include "Ai/BTT_CustomMoveTo_CPP.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h" // AI MoveTo 함수를 위해 필요

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

	// 블랙보드에서 목표물(TargetActor) 정보를 가져옵니다.
	UObject* TargetObject = BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey());
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (TargetActor)
	{
		// '1번 키' 테스트에서 작동했던 바로 그 함수를 호출합니다.
		UAIBlueprintHelperLibrary::SimpleMoveToActor(AIController, TargetActor);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}