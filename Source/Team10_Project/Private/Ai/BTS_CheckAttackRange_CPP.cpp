#include "Ai/BTS_CheckAttackRange_CPP.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBTS_CheckAttackRange_CPP::UBTS_CheckAttackRange_CPP()
{
	NodeName = TEXT("CPP_CheckAttackRange");
}

void UBTS_CheckAttackRange_CPP::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	// �����忡�� ��ǥ��(TargetActor) ������ �����ɴϴ�.
	UObject* TargetObject = BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey());
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (TargetActor)
	{
		APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
		if (ControllingPawn)
		{
			// �Ÿ��� ����ؼ� IsInAttackRange Ű ���� �����մϴ�.
			float Distance = FVector::Dist(TargetActor->GetActorLocation(), ControllingPawn->GetActorLocation());
			bool bIsInAttackRange = (Distance <= AttackRange);
			BlackboardComp->SetValueAsBool(FName("IsInAttackRange"), bIsInAttackRange);
		}
	}
	else
	{
		// Ÿ���� ������ false�� �����մϴ�.
		BlackboardComp->SetValueAsBool(FName("IsInAttackRange"), false);
	}
}