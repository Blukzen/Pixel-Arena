// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define Arena_Actor ""

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"

#include "ArenaActor.generated.h"

/**
 * 
 */
UCLASS()
class PIXELARENA_API AArenaActor : public APaperCharacter
{
	GENERATED_BODY()

	public:
		// Properties
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Arena_Actor) int MaxHealth; // Actors Max health
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Arena_Actor) int Health; // Actors Current health
		UPROPERTY(EditAnywhere, Category = Arena_Actor) int AttackDamage; // Actors damage stat
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Arena_Actor) bool CanDamage = true; // Whether or not the actor can currently be damaged
	
		// Functions
		UFUNCTION(BlueprintCallable, Category = Arena_Actor) virtual void Damage(int amount);
		UFUNCTION(BlueprintCallable, Category = Arena_Actor) void PlayFlipbook(UPaperFlipbook* flipbook, bool loop);

		// Events
		UFUNCTION(BlueprintImplementableEvent, Category = Arena_Actor) void OnDamage(int amount);
};
