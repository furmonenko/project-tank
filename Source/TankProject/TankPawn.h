#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "Containers/Deque.h"
#include <deque>
#include "TankPawn.generated.h"

USTRUCT()
struct FInputState
{
	GENERATED_BODY()
    
	float MoveValue;
	float TurnValue;
	float Timestamp;
	FVector Position; 
	FRotator Rotation;
};

UCLASS()
class TANKPROJECT_API ATankPawn : public ATurretPawn
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated)
	TArray<FInputState> InputBuffer;
	
	ATankPawn();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDied);

	FTimerHandle InterpolationTimerHandle;

	float StartTime = 0.f;
	
	FVector ServerPosition;
	FRotator ServerRotation;

	UFUNCTION(Client, Reliable)
	void ClientCorrectPosition(FVector CorrectPosition);

	UFUNCTION(Client, Reliable)
	void ClientCorrectRotation(FRotator CorrectRotation);
	
	UFUNCTION(BlueprintCallable)
	void Move(float ActionValue);
	
	UFUNCTION(BlueprintCallable)
	void Turn(float ActionValue);
	
	UFUNCTION(BlueprintCallable)
	void HandleInputMove(float ActionValue);

	UFUNCTION(BlueprintCallable)
	void HandleInputTurn(float ActionValue);
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FTankDied TankDied;

	virtual void TurretInit() override;
	
	virtual void OnHealthChanged(float CurrentHealth, float MaxHealth) override;
	
	UPROPERTY(ReplicatedUsing = OnRep_Ammo, VisibleAnywhere, BlueprintReadWrite,  Category = "Combat")
	int32 AmmoCount = 10;
	
	UFUNCTION(BlueprintCallable)
	void OnRep_Ammo();
	
	UPROPERTY(Replicated)
	FVector TankLocation;
	
	UFUNCTION(Server, Unreliable)
	void ServerMove(float ActionValue);
	
	UPROPERTY(Replicated)
	FRotator TankRotation;
	
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void ServerTurn(float ActionValue);
	
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientPlayFireEffects();
	
	void PlayLocalCameraShake();
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;
	
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
	
	UPROPERTY(BlueprintReadWrite)
	bool isMoving = false;

private:
	UPROPERTY()
	APlayerController* PlayerController = nullptr;
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
