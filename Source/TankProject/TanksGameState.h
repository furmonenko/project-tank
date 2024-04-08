#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TanksGameState.generated.h"

class ATowerPawn;
class ATurretPawn;
enum class ETeam : uint8;

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	Preparation UMETA(DisplayName = "Preparation"),
	InProgress UMETA(DisplayName = "InProgress"),
	Completed UMETA(DisplayName = "Completed")
};


UCLASS()
class TANKPROJECT_API ATanksGameState : public AGameState
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPreparationCountdownChanged, int32, CountdownTime);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTimeChanged, FString, GameTimeStr);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStarted);

	UPROPERTY(Replicated, BlueprintReadWrite, BlueprintAssignable)
	FOnGameTimeChanged GameTimeChanged;
	
	UPROPERTY(Replicated, BlueprintReadWrite, BlueprintAssignable)
	FGameStarted GameStarted;

	UPROPERTY(Replicated, BlueprintReadWrite, BlueprintAssignable)
	FOnPreparationCountdownChanged CountdownChanged;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentGamePhase, VisibleAnywhere, Category = "Game Phase")
	EGamePhase CurrentGamePhase = EGamePhase::Preparation;

	UFUNCTION()
	void OnRep_CurrentGamePhase();
	
	UPROPERTY(ReplicatedUsing = OnRep_PreparationCountdown, VisibleAnywhere, Category = "Game Phase")
	int32 PreparationCountdown = 10;

	UPROPERTY(ReplicatedUsing = OnRep_GameTimeCountdown, VisibleAnywhere, Category = "Game Phase")
	int32 GameTime = 10;

	UFUNCTION()
	void OnRep_GameTimeCountdown();
	
	UFUNCTION()
	void OnRep_PreparationCountdown();
	
	TMap<ETeam, int32> Teams;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<ATurretPawn*> Turrets;

	TSubclassOf<ATurretPawn> TurretPawnClass;
	
	void SetTeams();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
