#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CheckAttackRange_CPP.generated.h"

UCLASS(Blueprintable) // <--- �� �κ��� �����Ǿ����ϴ�.
class TEAM10_PROJECT_API UBTS_CheckAttackRange_CPP : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_CheckAttackRange_CPP();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// ���� ������ �����Ϳ��� ������ �� �ֵ��� ���� �߰�
	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 500.0f;
};