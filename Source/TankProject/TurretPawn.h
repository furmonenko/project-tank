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
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* ProjectileSpawnPoint;
	
	

};
