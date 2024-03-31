#include "BTService_UpdateTargetLocation.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTService_UpdateTargetLocation::UMyBTService_UpdateTargetLocation()
{
	NodeName = "Update Target Location";
}

void UMyBTService_UpdateTargetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp))
	{
		return;
	}
	
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey()));

	if (TargetActor)
	{
		FVector TargetLocation = TargetActor->GetActorLocation();
		BlackboardComp->SetValueAsVector("TargetLocation", TargetLocation);
	}
}
