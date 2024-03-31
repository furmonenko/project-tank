#include "BTService_UpgradeTargetTurretRotation.h"

#include "TowerPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"

UBTService_UpgradeTargetTurretRotation::UBTService_UpgradeTargetTurretRotation()
{
	NodeName = "Upgrade Target Rotation";
}

void UBTService_UpgradeTargetTurretRotation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp))
	{
		return;
	}
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
	{
		return;
	}
	
	APawn* Pawn = AIController->GetPawn();
	if (!IsValid(AIController))
	{
		return;
	}
	
	ATowerPawn* TowerPawn = Cast<ATowerPawn>(Pawn);
	if (!IsValid(TowerPawn))
	{
		return;
	}

	FVector TargetLocation = BlackboardComp->GetValueAsVector(GetSelectedBlackboardKey());
	
	TowerPawn->SetTargetLookRotation(TargetLocation);
}
