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
	virtual void Tick(float delta) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditDefaultsOnly)
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
	
	UFUNCTION(BlueprintCallable)
    virtual void Fire();

	UFUNCTION(BlueprintCallable)
	virtual void SetTargetLookRotation(FRotator Rotation) {};
	
	UPROPERTY(BlueprintReadWrite)
	FRotator TurretTargetRotation;
	
	UFUNCTION(BlueprintCallable)
	void RotateTurretSmooth() const;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	UHealthComponent* Health;
	
private:
	void SetTeamColor();
	
	UFUNCTION()
	TArray <FName> GetTurretAvailableSlotNames();
	
	UFUNCTION()
	TArray <FName> GetBaseAvailableSlotNames();
	
	UPROPERTY()
	UMaterialInstanceDynamic* M_TeamSlot;
};
