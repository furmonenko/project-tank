#include "HealthComponent.h"

#include "TurretPawn.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (IsValid(Owner))
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
}


void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth -= Damage;

	// In case damage is less than health.
	if (CurrentHealth <= 0.f)
	{
		ATurretPawn* DamagedTurret = Cast<ATurretPawn>(DamagedActor);

		if (IsValid(DamagedTurret))
		{
			DamagedTurret->OnTurretDeath();
		}
	}
}

