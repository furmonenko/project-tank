#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateTargetLocation.generated.h"

UCLASS()
class TANKPROJECT_API UMyBTService_UpdateTargetLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UMyBTService_UpdateTargetLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
