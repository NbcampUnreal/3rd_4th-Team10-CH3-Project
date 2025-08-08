#include "Ai/AIController_Monster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "Ai/Character_Monster.h"

AAIController_Monster::AAIController_Monster()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// 시야 설정 객체 생성
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		// 시야 반경 및 적 감지 설정
		SightConfig->SightRadius = 100000.0f;
		SightConfig->LoseSightRadius = 15000.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;

		// --- [추가된 부분] ---
		// 감지한 대상을 기억하는 시간을 설정합니다. 0으로 설정하면 무한히 기억합니다.
		SightConfig->SetMaxAge(0.0f);
		// --------------------

		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

		// 생성한 시야 설정을 Perception 컴포넌트에 적용
		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	}
}

// OnPossess 함수의 실제 내용 구현
void AAIController_Monster::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		BBComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
		BTComponent->StartTree(*BehaviorTreeAsset);
	}

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_Monster::OnTargetPerceptionUpdated);
}

// OnTargetPerceptionUpdated 함수의 실제 내용 구현
void AAIController_Monster::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
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