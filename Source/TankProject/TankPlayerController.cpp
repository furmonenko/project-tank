#include "TankPlayerController.h"
#include "HAL/IConsoleManager.h"
#include "TankPawn.h"
#include "TeamManager.h"

ATankPlayerController::ATankPlayerController()
{
	
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankPlayerController::ChangeTeam(const int TeamNumber)
{
	ETeam NewTeam = ETeam::Default;

	switch (TeamNumber)
	{
	case 1: NewTeam = ETeam::Team_1;
		break;
	case 2: NewTeam = ETeam::Team_2;
		break;
	case 3: NewTeam = ETeam::Team_3;
		break;
	case 4: NewTeam = ETeam::Team_4;
		break;
	default: GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid team number."));
		return;
	}


	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, GetPawn()->GetName());

	ControlledTank = Cast<ATankPawn>(GetPawn());
	if (!IsValid(ControlledTank))
	{
		return;
	}

	ControlledTank->ServerChangeTeam(NewTeam);	
}
