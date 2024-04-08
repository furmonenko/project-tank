#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATankPawn;

UCLASS()
class TANKPROJECT_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	ATankPawn* ControlledTank;
public:
	TUniquePtr<FAutoConsoleCommand> ChangeTeamCmd;
	
	ATankPlayerController();
	virtual void BeginPlay() override;

	UFUNCTION(Exec)
	void ChangeTeam(const int TeamNumber);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHUDAmmoCount(const int32 NewAmmo);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHUDHealth(float NewHealth, float MaxHealth);
};
