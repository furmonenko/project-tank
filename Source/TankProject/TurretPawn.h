#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "TeamManager.h"
#include "GameFramework/Pawn.h"
#include "TurretPawn.generated.h"

class UCapsuleComponent;
class UHealthComponent;

UCLASS()
class TANKPROJECT_API ATurretPawn : public APawn
{
    GENERATED_BODY()

public:
    // Constructor and Engine Overrides
    ATurretPawn();
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

    UFUNCTION()
    virtual void TurretInit();
    
    UFUNCTION(BlueprintCallable)
    void HandleFireInput();
    
    // Health Change Function
    UFUNCTION()
    virtual void OnHealthChanged(float CurrentHealth, float MaxHealth) {};

    // Team Functions
    UFUNCTION(Server, Reliable)
    void ServerChangeTeam(ETeam NewTeam);

    UFUNCTION()
    void OnRep_ChangeTeam();

    // Firing Functions
    UPROPERTY(BlueprintReadWrite)
    bool bCanFire = false;
    
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerFire();

    UFUNCTION()
    virtual void PlayFireEffects();

    // Turret Rotation Functions
    UFUNCTION(NetMulticast, Unreliable)
    void MulticastRotateTurret(const FRotator& NewRotation);

    UFUNCTION(BlueprintCallable)
    bool RotateTurretSmooth(float DeltaTime);

    UFUNCTION(Server, Unreliable)
    void ServerRotateTurret(float DeltaTime);

    UFUNCTION(Server, Unreliable)
    void ServerSetTargetLookRotation(FRotator NewRotation);

    virtual void SetTargetLookRotation() {};

protected:
    // Components
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UCapsuleComponent* CapsuleComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* TurretMesh;

    UPROPERTY(EditDefaultsOnly)
    UStaticMeshComponent* BaseMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    USceneComponent* ProjectileSpawnPoint;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UHealthComponent* HealthComponent;

    // Team and Movement Properties
    UPROPERTY(ReplicatedUsing = OnRep_ChangeTeam, EditAnywhere, BlueprintReadWrite, Category = "Team")
    ETeam Team = ETeam::Team_1;
    
    UPROPERTY(ReplicatedUsing = OnRep_bCanMove, BlueprintReadWrite)
    bool bCanMove = true;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
    FRotator TurretTargetRotation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float RotationRate = 2.f;

public:
    ETeam GetTeam() const { return Team; }
    bool GetCanMove() const { return bCanMove; }
    void SetCanMove(bool NewStatus) { bCanMove = NewStatus; }
    
protected:
    // Team Color Properties
    UPROPERTY()
    FLinearColor TeamColor;

    UPROPERTY(EditDefaultsOnly, Category = "TeamColor")
    UMaterialInterface* DefaultMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "TeamColor")
    FName MaterialParameterName;

    UPROPERTY(EditDefaultsOnly, Category = "TeamColor", meta = (GetOptions = "GetTurretAvailableSlotNames"))
    FName TurretMeshSlotToColor;

    UPROPERTY(EditDefaultsOnly, Category = "TeamColor", meta = (GetOptions = "GetBaseAvailableSlotNames"))
    FName BaseMeshSlotToColor;

    // Projectile and Effects
    UPROPERTY(EditAnywhere, Category = "Projectile")
    TSubclassOf<AProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    UParticleSystem* SmokeEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* ShotSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* TurretRotationAudioComponent;
    
    // Helper Functions
    UFUNCTION()
    TArray<FName> GetTurretAvailableSlotNames() const;

    UFUNCTION()
    TArray<FName> GetBaseAvailableSlotNames() const;

    UPROPERTY()
    UMaterialInstanceDynamic* M_TeamSlot;

    void SetTeamColor();

    // Replication
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangedTeam, AActor*, TurretPawn);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTurretDiedDelegate, AActor*, DiedTurret, AActor*, KillerTurret);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWonGame);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLostGame);
    
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FWonGame WonGame;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FLostGame LostGame;

    UPROPERTY(BlueprintCallable, Category = "Events")
    FTurretDiedDelegate TurretDied;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FChangedTeam ChangedTeam;

    UFUNCTION(BlueprintImplementableEvent)
    void OnTurretDeath(AActor* KillerTurretPawn);

    UFUNCTION(BlueprintImplementableEvent)
    void OnRep_bCanMove();
};
