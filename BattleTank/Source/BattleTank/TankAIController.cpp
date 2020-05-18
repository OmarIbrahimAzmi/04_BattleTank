// Copyright Press ST:art.


#include "BattleTank.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Tank.h" // So we can impliment OnDeath
#include "TankAIController.h"
// Depends on movement component via pathfinding system.


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		// Subscribe our local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossedTankDeath);
	}
}

void ATankAIController::OnPossedTankDeath()
{
	//DetachFromControllerPendingDestroy();
	UE_LOG(LogTemp, Error, TEXT("AI Tank died."));
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();
	if (!ensure(PlayerTank && ControlledTank)) { return; }

	// Move Towards the player
	MoveToActor(PlayerTank, AcceptanceRadius); // TODO Check radius is in cm.

	// Aim towards the player
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	// if Aiming or Locked then fire
	if (AimingComponent->GetFiringState() == EFiringState::Locked)
	{
		AimingComponent->Fire(); // TODO Don't fire every frame
	}
}