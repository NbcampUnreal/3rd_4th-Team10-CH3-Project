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

	// 블랙보드에서 목표물(TargetActor) 정보를 가져옵니다.
	UObject* TargetObject = BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey());
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (TargetActor)
	{
		APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
		if (ControllingPawn)
		{
			// 거리를 계산해서 IsInAttackRange 키 값을 설정합니다.
			float Distance = FVector::Dist(TargetActor->GetActorLocation(), ControllingPawn->GetActorLocation());
			bool bIsInAttackRange = (Distance <= AttackRange);
			BlackboardComp->SetValueAsBool(FName("IsInAttackRange"), bIsInAttackRange);
		}
	}
	else
	{
		// 타겟이 없으면 false로 설정합니다.
		BlackboardComp->SetValueAsBool(FName("IsInAttackRange"), false);
	}
}