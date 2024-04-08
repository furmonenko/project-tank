#include "TanksGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TurretPawn.h"

void ATanksGameState::OnRep_CurrentGamePhase()
{
	if (CurrentGamePhase == EGamePhase::InProgress)
	{
		GameStarted.Broadcast();
	}
}

void ATanksGameState::OnRep_GameTimeCountdown()
{
	if (CurrentGamePhase == EGamePhase::InProgress)
	{
		int Minutes = GameTime / 60;
		int Seconds = GameTime % 60;
		
		FString FormattedTime = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		
		GameTimeChanged.Broadcast(FormattedTime);
	}
}

void ATanksGameState::OnRep_PreparationCountdown()
{
	CountdownChanged.Broadcast(PreparationCountdown);
}

void ATanksGameState::SetTeams()
{
	TArray<AActor*> TurretPawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATurretPawn::StaticClass(), TurretPawns);

	for (AActor* Actor : TurretPawns)
	{
		ATurretPawn* TurretPawn = Cast<ATurretPawn>(Actor);

		if (!IsValid(TurretPawn))
		{
			return;
		}
		
		Turrets.Add(TurretPawn);

		if (TurretPawn->GetTeam() != ETeam::Towers && TurretPawn->GetTeam() != ETeam::Default)
		{
			Teams.FindOrAdd(TurretPawn->GetTeam()) = 0;

			const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETeam"), true);
			if (EnumPtr != nullptr)
			{
				FString TeamName = EnumPtr->GetNameStringByValue((int64)TurretPawn->GetTeam());
				UE_LOG(LogTemp, Warning, TEXT("TurretPawn of Team %s added with initial score 0."), *TeamName);
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Total teams added: %d"), Teams.Num());
}

void ATanksGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATanksGameState, PreparationCountdown);
	DOREPLIFETIME(ATanksGameState, CountdownChanged);
	DOREPLIFETIME(ATanksGameState, CurrentGamePhase);
	DOREPLIFETIME(ATanksGameState, GameTime);
}
