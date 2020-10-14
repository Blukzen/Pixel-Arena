// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaProjectile.h"

#include "Kismet/GameplayStatics.h"

/**
 * Constructor.
 */
AArenaProjectile::AArenaProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

/**
 * Fires a projectile at the direction of the character.
 */
void AArenaProjectile::FireAtPlayer()
{
	FVector playerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	FVector direction = playerLocation - GetActorLocation();
	Fire(direction);
}

