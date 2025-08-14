#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h" 
#include "AIController_Monster.generated.h"

                                   
UCLASS()
class TEAM10_PROJECT_API AAIController_Monster : public ADetourCrowdAIController
{
    GENERATED_BODY()

public:
    AAIController_Monster();

protected:
    // 몬스터에 빙의할 때 호출될 함수 (선언)
    virtual void OnPossess(APawn* InPawn) override;

private:
    // 비헤이비어 트리와 블랙보드 컴포넌트
    UPROPERTY()
    TObjectPtr<class UBehaviorTreeComponent> BTComponent;
    UPROPERTY()
    TObjectPtr<class UBlackboardComponent> BBComponent;

    // 시야(Perception) 컴포넌트
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UAIPerceptionComponent> AIPerceptionComponent;

    // 에디터에서 설정할 비헤이비어 트리 에셋
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    TObjectPtr<class UBehaviorTree> BehaviorTreeAsset;

    // AI의 눈에 무언가 감지되었을 때 호출될 함수 (선언)
    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};