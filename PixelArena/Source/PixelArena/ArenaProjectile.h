// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "ArenaActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "ArenaProjectile.generated.h"

UCLASS()
class PIXELARENA_API AArenaProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArenaProjectile();

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile) int Damage; // Amount of damage this projectile deals.

	// Events
	UFUNCTION(BlueprintImplementableEvent, Category = Projectile) void Fire(FVector direction); // Event called when the projectile should begin to shoot/move.
	
	// Functions
	void FireAtPlayer();


};
