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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* MovementSmokeParticleSystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* MovementSmoke;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 0.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxSpeed = 200.f;;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float TurningSpeed = 50.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float AccelerationRate = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DecelerationRate = 5.f;
	
	UFUNCTION(BlueprintCallable)
	void Move(float ActionValue);

	UFUNCTION(BlueprintCallable)
	void Turn(float ActionValue);

	UFUNCTION(BlueprintCallable)
	void Die();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Auido")
	USoundBase* DeathSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	UAudioComponent* MovementAudioComponent;
	
	// virtual void Fire() override;
	virtual void SetTargetLookRotation(FRotator Rotation) override;

	UPROPERTY(BlueprintReadWrite)
	bool isMoving = false;

private:
	UPROPERTY()
	APlayerController* PlayerController = nullptr;
};
