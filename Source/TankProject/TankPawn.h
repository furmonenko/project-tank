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


	UPROPERTY(ReplicatedUsing=OnRep_TankLocation)
	FVector TankLocation;

	UFUNCTION()
	void OnRep_TankLocation();
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerMove(float ActionValue);
	
	UPROPERTY(ReplicatedUsing=OnRep_TankRotation)
	FRotator TankRotation;

	UFUNCTION()
	void OnRep_TankRotation();
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerTurn(float ActionValue);
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* MovementSmokeParticleSystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* MovementSmoke;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxSpeed = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float TurningSpeed = 50.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float AccelerationRate = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DecelerationRate = 5.f;
	
	UFUNCTION(BlueprintCallable)
	void Die();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Auido")
	USoundBase* DeathSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	UAudioComponent* MovementAudioComponent;
	
	UFUNCTION(BlueprintCallable)
	virtual void SetTargetLookRotation() override;

	UFUNCTION(Server, Reliable)
	void ServerSetTargetLookRotation(FRotator NewRotation);
	
	UPROPERTY(BlueprintReadWrite)
	bool isMoving = false;

private:
	UPROPERTY()
	APlayerController* PlayerController = nullptr;
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
