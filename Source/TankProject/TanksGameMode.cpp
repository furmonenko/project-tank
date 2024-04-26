// Fill out your copyright notice in the Description page of Project Settings.


#include "TanksGameMode.h"

#include "TanksGameState.h"
#include "TeamManager.h"
#include "TurretPawn.h"

ATanksGameMode::ATanksGameMode()
{
	bUseSeamlessTravel = true;
}

void ATanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	ATanksGameState* TanksGameState = GetGameState<ATanksGameState>();
	if (TanksGameState)
	{
		GetWorldTimerManager().SetTimer(PreparationTimerHandle, this, &ATanksGameMode::HandlePreparationCountdown, 1.0f,
		                                true);
	}
}

void ATanksGameMode::InitGame()
{
	ATanksGameState* TanksGameState = GetGameState<ATanksGameState>();
	if (!IsValid(TanksGameState))
	{
		return;
	}

	TanksGameState->SetTeams();

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	World->GetTimerManager().SetTimer(GameTimerHandle, [this, TanksGameState]()
	{
		if (TanksGameState->GameTime > 0)
		{
			--TanksGameState->GameTime;
			TanksGameState->OnRep_GameTimeCountdown();

			if (TanksGameState->GameTime == 0)
			{
				OnGameTimeExpired();
			}
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);
		}
	}, 1.0f, true, 0.0f);

	BindTurretsToOnTurretDied();
}

void ATanksGameMode::OnGameTimeExpired()
{
	ATanksGameState* TanksGameState = GetGameState<ATanksGameState>();
	if (!GameState)
	{
		return;
	}

	ETeam WinningTeam = ETeam::Default;
	int32 HighestScore = 0;

	for (const auto& Team : TanksGameState->Teams)
	{
		if (Team.Key != ETeam::Towers && Team.Value > HighestScore)
		{
			HighestScore = Team.Value;
			WinningTeam = Team.Key;
		}
	}

	FString WinningTeamName;
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETeam"), true);
	if (EnumPtr != nullptr && WinningTeam != ETeam::Default)
	{
		WinningTeamName = EnumPtr->GetNameStringByValue((int64)WinningTeam);
	}

	FString Message = FString::Printf(TEXT("The winning team is: %s with %d points!"), *WinningTeamName, HighestScore);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);
	}

	for (const auto& Turret : TanksGameState->Turrets)
	{
		if (Turret->GetTeam() == WinningTeam)
		{
			Turret->WonGame.Broadcast();
		}
		else
		{
			Turret->LostGame.Broadcast();
		}


		Turret->SetCanMove(false);
		Turret->OnRep_bCanMove();

		TanksGameState->CurrentGamePhase = EGamePhase::Preparation;
		FTimerHandle RestartTimerHandle;
		GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &ATanksGameMode::RestartGame, 5.0f, false);
	}
}

void ATanksGameMode::AddScore(ETeam Team, int32 Points)
{
	ATanksGameState* TanksGameState = GetGameState<ATanksGameState>();
	if (GameState)
	{
		int32& TeamScore = TanksGameState->Teams.FindOrAdd(Team);
		TeamScore += Points;

		if (TeamScore <= 0)
		{
			TeamScore = 0;
		}
	}
}

void ATanksGameMode::BindTurretsToOnTurretDied()
{
	ATanksGameState* TanksGameState = GetGameState<ATanksGameState>();

	if (!IsValid(GameState))
	{
		return;
	}

	if (TanksGameState->Turrets.IsEmpty())
	{
		return;
	}

	for (auto Turret : TanksGameState->Turrets)
	{
		Turret->TurretDied.AddDynamic(this, &ATanksGameMode::OnTurretDied);
	}
}

void ATanksGameMode::OnTurretDied(AActor* DiedTurretActor, AActor* KillerTurretActor)
{
	if (!IsValid(DiedTurretActor) || !IsValid(KillerTurretActor))
	{
		return;
	}

	ATurretPawn* DiedTurret = Cast<ATurretPawn>(DiedTurretActor);
	ATurretPawn* KillerTurret = Cast<ATurretPawn>(KillerTurretActor);

	if (!IsValid(DiedTurret) || !IsValid(KillerTurret))
	{
		return;
	}

	ETeam DeadTeam = DiedTurret->GetTeam();
	ETeam KillerTeam = KillerTurret->GetTeam();

	FString KillerTeamName;
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETeam"), true);
	if (EnumPtr != nullptr && KillerTeam != ETeam::Default)
	{
		KillerTeamName = EnumPtr->GetNameStringByValue((int64)KillerTeam);
	}

	if (DeadTeam == ETeam::Towers)
	{
		AddScore(KillerTeam, 5);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
		                                 FString::Printf(
			                                 TEXT("Tower destroyed by Team %s. +5 points."),
			                                 *KillerTeamName));
	}
	else
	{
		if (DeadTeam == KillerTeam)
		{
			AddScore(KillerTeam, -15);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			                                 FString::Printf(TEXT("You killed your Ally!. -15 points.")));
		}
		else
		{
			AddScore(KillerTeam, 10);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			                                 FString::Printf(
				                                 TEXT("Turret destroyed by Team %s. +10 points."),
				                                 *KillerTeamName));
		}
	}

	FTimerHandle TimerHandle;
	float DelayInSeconds = GetWorld()->GetDeltaSeconds();
	FTimerDelegate TimerDel = FTimerDelegate::CreateLambda([DiedTurretActor]()
	{
		if (IsValid(DiedTurretActor))
		{
			DiedTurretActor->Destroy();
		}
	});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, DelayInSeconds, false);
}

void ATanksGameMode::HandlePreparationCountdown()
{
	ATanksGameState* TanksGameState = GetGameState<ATanksGameState>();
	if (TanksGameState)
	{
		if (TanksGameState->PreparationCountdown > 0)
		{
			--TanksGameState->PreparationCountdown;
			TanksGameState->OnRep_PreparationCountdown();
		}
		else
		{
			GetWorldTimerManager().ClearTimer(PreparationTimerHandle);
			TanksGameState->CurrentGamePhase = EGamePhase::InProgress;
			TanksGameState->OnRep_CurrentGamePhase();
			InitGame();
		}
	}
}
