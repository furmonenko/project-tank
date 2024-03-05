// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "TowerPawn.generated.h"

class ATankPawn;

UCLASS()
class TANKPROJECT_API ATowerPawn : public ATurretPawn
{
	GENERATED_BODY()

public:
	ATowerPawn();
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class ATankPawn* Tank = nullptr;

	float FireRate = 2.f;
	
	FTimerHandle FireTimerHandle;
};
