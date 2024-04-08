#include "HealthComponent.h"
#include "TurretPawn.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	HealthChanged.Broadcast(CurrentHealth, MaxHealth);
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetIsReplicated(true);

	AActor* Owner = GetOwner();
	if (IsValid(Owner))
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}

	OnRep_Health();
}


void UHealthComponent::OnRep_Health()
{
	HealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                  AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth -= Damage;
	
	if (CurrentHealth <= 0.f)
	{
		ATurretPawn* DamagedTurret = Cast<ATurretPawn>(DamagedActor);
		ATurretPawn* AttackingTurretPawn = Cast<ATurretPawn>(InstigatedBy->GetPawn()); 
		
		if (IsValid(DamagedTurret))
		{
			DamagedTurret->OnTurretDeath(AttackingTurretPawn);
		}
	}

	if (GetOwner()->HasAuthority())
	{
		OnRep_Health();
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}


