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
	
	if (!ControlledTurret)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "No Turret");
	}

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
	ControlledTurret = Cast<ATurretPawn>(GetPawn());

	if (!IsValid(EnemyTank))
	{
		return;
	}
	
	if (Stimulus.WasSuccessfullySensed())
	{
		PerceivedEnemies.Push(EnemyTank);
		EnemyTank->OnChangedTeam.AddDynamic(this, &AAITowerController::UpdateClosestEnemyAsTarget);
	}
	else
	{
		EnemyTank->OnChangedTeam.RemoveDynamic(this, &AAITowerController::UpdateClosestEnemyAsTarget);
		PerceivedEnemies.Remove(EnemyTank);
	}
	
	UpdateClosestEnemyAsTarget();
}

void AAITowerController::UpdateClosestEnemyAsTarget()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "UpdateClosestEnemyAsTarget");
	
    AActor* ClosestEnemy = nullptr;
	FVector MyLocation = GetPawn()->GetActorLocation();
    float ClosestDistanceSquared = MAX_FLT;
    
    for (const auto& Enemy : PerceivedEnemies)
    {
        float DistanceSquared = FVector::DistSquared(MyLocation, Enemy->GetActorLocation());
        if (DistanceSquared < ClosestDistanceSquared)
        {
			ATurretPawn* EnemyTurret = Cast<ATurretPawn>(Enemy);

        	if (!IsValid(EnemyTurret))
        	{
        		return;
        	}

        	if (!FTeamManager::IsEnemy(ControlledTurret, EnemyTurret))
        	{
        		GetBlackboardComponent()->ClearValue("TargetTank");
        		continue;
        	}
        	
            if (LineOfSightTo(EnemyTurret))
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
    }
    else if (!ClosestEnemy)
    {
        GetBlackboardComponent()->ClearValue("TargetTank");
    }
}



