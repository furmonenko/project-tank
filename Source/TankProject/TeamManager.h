#pragma once

#include "CoreMinimal.h"

class ATurretPawn;

UENUM(BlueprintType)
enum class ETeam : uint8
{
	Team_1 UMETA(DisplayName = "Team 1"),
	Team_2 UMETA(DisplayName = "Team 2"),
	Team_3 UMETA(DisplayName = "Team 3"),
	Team_4 UMETA(DisplayName = "Team 4"),
	Towers UMETA(DisplayName = "Towers"),

	Default
};

class TANKPROJECT_API FTeamManager
{
public:
	static FLinearColor GetTeamColor(ETeam TeamAffiliation);
	static FString GetTeamColorName(ETeam TeamAffiliation);
	
	static bool IsEnemy(const ATurretPawn* FirstTurret, const ATurretPawn* SecondTurret);
	
private:
	inline static FLinearColor Red = FLinearColor::Red;
	inline static FLinearColor Green = FLinearColor::Green;
	inline static FLinearColor Blue = FLinearColor::Blue;
	inline static FLinearColor Yellow = FLinearColor::Yellow;
	inline static FLinearColor Neutral = FLinearColor::Gray;
};
