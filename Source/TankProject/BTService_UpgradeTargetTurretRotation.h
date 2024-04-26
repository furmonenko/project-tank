#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTService_UpgradeTargetTurretRotation.generated.h"

UCLASS()
class TANKPROJECT_API UBTService_UpgradeTargetTurretRotation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_UpgradeTargetTurretRotation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
