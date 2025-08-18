#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack.generated.h"

// --- 추가된 부분 ---
// UBehaviorTreeComponent 클래스를 미리 알려줍니다. (전방 선언)
class UBehaviorTreeComponent;
// -----------------

UCLASS(Blueprintable)
class TEAM10_PROJECT_API UBTT_Attack : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTT_Attack();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    // --- 추가된 부분 ---
    // 몬스터의 공격 종료 신호를 받을 함수를 선언합니다.
    UFUNCTION()
    void OnAttackFinished(UBehaviorTreeComponent* OwnerComp);
    // -----------------
};