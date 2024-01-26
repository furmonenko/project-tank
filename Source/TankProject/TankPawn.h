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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	double MovementSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	double MaxSpeed;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	double AccelerationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	double DecelerationRate;
	
	UFUNCTION(BlueprintCallable)
	void Move(float ActionValue);

	UFUNCTION(BlueprintCallable)
	void Turn(float ActionValue);
	
	UFUNCTION(BlueprintCallable)
	void Look(FVector2D ActionValue);
	
	UFUNCTION(BlueprintCallable)
	void Fire();

private:
	UPROPERTY()
	APlayerController* PlayerController;
};
