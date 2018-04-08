// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (!Turret)
		return;

	if (!Barrel)
		return;

	FVector LaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	bool IsProjectileVelocityValid = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		OUT LaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	if (IsProjectileVelocityValid)
	{
		FVector AimDirection = LaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		//UE_LOG(LogTemp, Warning, TEXT("%f: Aim solution found: %s"), GetWorld()->GetTimeSeconds(), *AimDirection.ToString());
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("%f: NO Aim solution found"), GetWorld()->GetTimeSeconds());
	}
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel * BarrelToSet)
{
	Barrel = BarrelToSet;

	if (!Barrel)
	{
		UE_LOG(LogTemp, Error, TEXT("Barrel reference set to null! Make sure you've set it in BP."));
	}
}

void UTankAimingComponent::SetTurretReference(UTankTurret * TurretToSet)
{
	Turret = TurretToSet;

	if (!Turret)
	{
		UE_LOG(LogTemp, Error, TEXT("Turret reference set to null! Make sure you've set it in BP."));
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimRotator - BarrelRotator;
	
	Turret->Rotate(DeltaRotator.Yaw);
	Barrel->Elevate(DeltaRotator.Pitch);
}

