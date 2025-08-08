#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CheckAttackRange_CPP.generated.h"

UCLASS(Blueprintable) // <--- 이 부분이 수정되었습니다.
class TEAM10_PROJECT_API UBTS_CheckAttackRange_CPP : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_CheckAttackRange_CPP();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 공격 범위를 에디터에서 조절할 수 있도록 변수 추가
	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 500.0f;
};