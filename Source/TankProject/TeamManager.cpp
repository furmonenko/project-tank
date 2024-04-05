#include "TeamManager.h"
#include "TurretPawn.h"

FLinearColor FTeamManager::GetTeamColor(ETeam TeamAffiliation)
{
	switch (TeamAffiliation)
	{
	case ETeam::Team_1:
		return Red;
	case ETeam::Team_2:
		return Green;
	case ETeam::Team_3:
		return Blue;
	case ETeam::Team_4:
		return Yellow;
	default:
		return Neutral;
	}
}

FString FTeamManager::GetTeamColorName(ETeam TeamAffiliation)
{
	switch (TeamAffiliation)
	{
	case ETeam::Team_1: return TEXT("Red");
	case ETeam::Team_2: return TEXT("Green");
	case ETeam::Team_3: return TEXT("Blue");
	case ETeam::Team_4: return TEXT("Yellow");
	case ETeam::Towers: return TEXT("Neutral");
	default: return TEXT("Unknown");
	}
}

bool FTeamManager::IsEnemy(const ATurretPawn* FirstTurret, const ATurretPawn* SecondTurret)
{
	if (!IsValid(FirstTurret) || !IsValid(SecondTurret))
	{
		return false;
	}

	ETeam FirstTeam = FirstTurret->Team;
	ETeam SecondTeam = SecondTurret->Team;

	// Assuming everyone is enemy to everyone
	return FirstTeam != SecondTeam;
}
