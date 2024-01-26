#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TurretPawn.generated.h"

UCLASS()
class TANKPROJECT_API ATurretPawn : public APawn
{
	GENERATED_BODY()

public:
	ATurretPawn();

protected:
	virtual void BeginPlay() override;
	void RotateTurretSmooth() const;

	FRotator TurretTargetRotation;
	
public:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly)
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

private:
	void SetTeamColor();
	
	UFUNCTION()
	TArray <FName> GetTurretAvailableSlotNames();
	
	UFUNCTION()
	TArray <FName> GetBaseAvailableSlotNames();
	
	UPROPERTY()
	UMaterialInstanceDynamic* M_TeamSlot;
};
