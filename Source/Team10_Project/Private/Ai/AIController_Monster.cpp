#include "Ai/AIController_Monster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MyCharacter.h" // <--- [수정] 플레이어 클래스를 알아야 하므로 이 헤더를 추가합니다.

AAIController_Monster::AAIController_Monster()
{
    BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
    BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    if (SightConfig)
    {
        SightConfig->SightRadius = 100000.0f;
        SightConfig->LoseSightRadius = 15000.0f;
        SightConfig->PeripheralVisionAngleDegrees = 360.0f;
        SightConfig->SetMaxAge(0.0f);

        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

        AIPerceptionComponent->ConfigureSense(*SightConfig);
        AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
    }
}

void AAIController_Monster::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (BehaviorTreeAsset)
    {
        BBComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
        RunBehaviorTree(BehaviorTreeAsset);
    }

    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_Monster::OnTargetPerceptionUpdated);
}

// --- [수정된 부분] ---
// OnTargetPerceptionUpdated 함수의 내용을 수정합니다.
void AAIController_Monster::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    // 1. 감지된 액터가 플레이어 캐릭터인지 확인합니다.
    AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(Actor);

    // 2. 만약 플레이어 캐릭터가 아니라면 (다른 몬스터이거나 다른 물체라면), 아무것도 하지 않고 무시합니다.
    if (!PlayerCharacter)
    {
        return;
    }

    // 3. 오직 플레이어 캐릭터일 때만, 블랙보드에 TargetActor로 저장하거나 지웁니다.
    if (Stimulus.WasSuccessfullySensed())
    {
        // 성공적으로 봤다면 블랙보드의 TargetActor에 저장
        BBComponent->SetValueAsObject(FName("TargetActor"), Actor);
    }
    else
    {
        // 놓쳤다면 블랙보드의 TargetActor를 비움
        BBComponent->ClearValue(FName("TargetActor"));
    }
}