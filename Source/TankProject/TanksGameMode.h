#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TanksGameMode.generated.h"

class ATanksGameState;
class ATurretPawn;
enum class ETeam : uint8;

UCLASS()
class TANKPROJECT_API ATanksGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATanksGameMode();
	virtual void BeginPlay() override;

	FTimerHandle PreparationTimerHandle;

	UFUNCTION()
	void HandlePreparationCountdown();

	UFUNCTION(BlueprintCallable)
	void InitGame();

	void OnGameTimeExpired();

	UFUNCTION()
	void OnTurretDied(AActor* DiedTurretActor, AActor* KillerTurretActor);

	void AddScore(ETeam Team, int32 Points);
	void BindTurretsToOnTurretDied();

	FTimerHandle GameTimerHandle;
};
