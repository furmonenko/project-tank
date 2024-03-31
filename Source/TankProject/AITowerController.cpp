#include "AITowerController.h"

#include "TankPawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

AAITowerController::AAITowerController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BT");
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("BB");

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception Component");
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAITowerController::OnTargetPerceptionUpdated);
}

void AAITowerController::BeginPlay()
{
	Super::BeginPlay();
	

	if(IsValid(BehaviorTreeAsset))
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);
	}
}

void AAITowerController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || !AIPerceptionComponent) return;

	ATankPawn* EnemyTank = Cast<ATankPawn>(Actor);

	if (!IsValid(EnemyTank))
	{
		return;
	}
	
	if (Stimulus.WasSuccessfullySensed())
	{
		PerceivedEnemies.Push(EnemyTank);
	}
	else
	{
		PerceivedEnemies.Remove(EnemyTank);
	}
	
	UpdateClosestEnemyAsTarget();
}

void AAITowerController::UpdateClosestEnemyAsTarget()
{
    AActor* ClosestEnemy = nullptr;
    float ClosestDistanceSquared = MAX_FLT;
    FVector MyLocation = GetPawn()->GetActorLocation();
    
    for (const auto& Enemy : PerceivedEnemies)
    {
        float DistanceSquared = FVector::DistSquared(MyLocation, Enemy->GetActorLocation());
        if (DistanceSquared < ClosestDistanceSquared)
        {
            if (LineOfSightTo(Enemy))
            {
                ClosestEnemy = Enemy;
                ClosestDistanceSquared = DistanceSquared;
            }
        }
    }
    
    AActor* CurrentTarget = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject("TargetTank"));
    if (IsValid(ClosestEnemy) && ClosestEnemy != CurrentTarget)
    {
        GetBlackboardComponent()->SetValueAsObject("TargetTank", ClosestEnemy);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("New Closest Target: %s"), *ClosestEnemy->GetName()));
    }
    else if (!ClosestEnemy)
    {
        GetBlackboardComponent()->ClearValue("TargetTank");
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Targets in Sight"));
    }
}



