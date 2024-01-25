#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKPROJECT_API ATankPawn : public ATurretPawn
{
	GENERATED_BODY()

public:
	ATankPawn();
	
	UFUNCTION(BlueprintCallable)
	void Move(FVector2D ActionValue);

	UFUNCTION(BlueprintCallable)
	void Look(FVector2D ActionValue);
	
	UFUNCTION(BlueprintCallable)
	void Fire();
};
