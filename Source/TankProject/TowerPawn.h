#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "TowerPawn.generated.h"

UCLASS()
class TANKPROJECT_API ATowerPawn : public ATurretPawn
{
	GENERATED_BODY()

public:
	ATowerPawn();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class ATankPawn* Tank = nullptr;

	UFUNCTION()
	void SetTargetLookRotation(const FVector& TargetLocation);
};
