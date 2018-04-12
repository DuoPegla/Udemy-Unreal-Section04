// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankPlayerController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("AIController begin play"));

	ATank* AITank = GetAITank();
	if (AITank)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found AI tank: %s"), *AITank->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No AI tank found!"));
	}

	ATank* PlayerTank = GetPlayerTank();
	if (PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI tank found player tank: %s"), *PlayerTank->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AI tank couldn't find player tank!"));
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ATank* PlayerTank = GetPlayerTank();
	if (!PlayerTank)
	{
		return;
	}

	ATank* AITank = GetAITank();
	if (!AITank)
	{
		return;
	}

	AITank->AimAt(PlayerTank->GetActorLocation());

	AITank->Fire();
}

ATank* ATankAIController::GetAITank() const
{
	return Cast<ATank>(GetPawn());
}

ATank * ATankAIController::GetPlayerTank() const
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("No player controller found!"));
		return nullptr;
	}

	ATankPlayerController* TankPlayerController = Cast<ATankPlayerController>(PlayerController);

	if (!TankPlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("Player controller is not a tank player controller!"));
		return nullptr;
	}
	
	return TankPlayerController->GetControlledTank();
}
