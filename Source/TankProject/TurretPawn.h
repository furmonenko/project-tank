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
	
public:
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	
	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "TeamColor", meta = (GetOptions = "GetAvailableSlotNames"))
	FName SlotToColor;
	
	UPROPERTY(EditAnywhere, Category = "TeamColor")
	FName MaterialParameterName;
	
	UPROPERTY(EditAnywhere, Category = "TeamColor")
	FLinearColor TeamColor;

private:
	void SetTeamColor();
	TArray<FName>GetAvailableSlotNames();
	
	UPROPERTY()
	UMaterialInstanceDynamic* M_TeamSlot;
};
