#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "GameFramework/Pawn.h"
#include "TurretPawn.generated.h"

class UHealthComponent;

UCLASS()
class TANKPROJECT_API ATurretPawn : public APawn
{
	GENERATED_BODY()

public:
	ATurretPawn();
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;

	// Material, which M_TeamSlot is gonna be made of.
	UPROPERTY(EditDefaultsOnly, Category = "TeamColor")
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "TeamColor")
	FLinearColor TeamColor;
	
	UPROPERTY(EditDefaultsOnly, Category = "TeamColor")
	FName MaterialParameterName;

	UPROPERTY(EditDefaultsOnly, Category = "TeamColor", meta = (GetOptions = "GetTurretAvailableSlotNames"))
	FName TurretMeshSlotToColor;

	UPROPERTY(EditDefaultsOnly, Category = "TeamColor", meta = (GetOptions = "GetBaseAvailableSlotNames"))
	FName BaseMeshSlotToColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float RotationRate = 2.f;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerFire();
	
	UFUNCTION()
	virtual void PlayFireEffects();
	
	virtual void SetTargetLookRotation() {};
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	FRotator TurretTargetRotation;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRotateTurret(const FRotator& NewRotation);
	
	UFUNCTION(BlueprintCallable)
	bool RotateTurretSmooth(const float delta);

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UHealthComponent* Health;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* SmokeEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* ShotSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	UAudioComponent* TurretRotationAudioComponent;

private:
	void SetTeamColor();
	
	UFUNCTION()
	TArray <FName> GetTurretAvailableSlotNames();
	
	UFUNCTION()
	TArray <FName> GetBaseAvailableSlotNames();
	
	UPROPERTY()
	UMaterialInstanceDynamic* M_TeamSlot;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnTurretDeath();
	
	UFUNCTION(Server, Reliable)
	void ServerRotateTurret(float delta);
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	UFUNCTION(Server, Reliable)
	void ServerSetTargetLookRotation(FRotator NewRotation);
};
