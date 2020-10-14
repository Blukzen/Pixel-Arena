// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define ARENA_BOSS "Arena Boss"

#include "CoreMinimal.h"

#include <string>

#include "ArenaActor.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "ArenaBoss.generated.h"


/**
 * Boss State Types
 */
UENUM(BlueprintType)
enum BossState {
	BossIdle UMETA(DisplayName = "Boss Idle"),
	BossAttacking UMETA(DisplayName = "Boss Attacking"),
	BossAbility UMETA(DisplayName = "Boss Ability")
};

UCLASS()
class PIXELARENA_API AArenaBoss : public AArenaActor
{
	GENERATED_BODY()
public:
	// Constructor
	AArenaBoss();

	// Properties
	UPROPERTY(EditAnywhere, Category = ARENA_BOSS) float AttackSpeed = 100; // Attack speed in milliseconds.
	UPROPERTY(EditAnywhere, Category = ARENA_BOSS) UBoxComponent* VisionBox; // Size of the bosses vision box.
	UPROPERTY(BlueprintReadWrite, Category = ARENA_BOSS) bool bPlayerVisible = false; // Whether the player is within the boss vision box.
	UPROPERTY(BlueprintReadWrite, Category = ARENA_BOSS) TEnumAsByte<BossState> CurrentState = BossIdle; // Bosses current state
	UPROPERTY(BlueprintReadWrite, Category = ARENA_BOSS) AArenaActor* Character; // Reference to the current players character

	// Events
	UFUNCTION(BlueprintImplementableEvent, Category = ARENA_BOSS) void IdleState();
	UFUNCTION(BlueprintImplementableEvent, Category = ARENA_BOSS) void AttackState();
	UFUNCTION(BlueprintImplementableEvent, Category = ARENA_BOSS) void AbilityState();
	UFUNCTION(BlueprintImplementableEvent, Category =  ARENA_BOSS) void AnimationFinished();
	
	// Functions
	UFUNCTION(BlueprintCallable, Category = ARENA_BOSS) virtual void Attack();
	UFUNCTION(BlueprintCallable, Category =  ARENA_BOSS) virtual void Ability();

protected:
	// Functions
	virtual void Tick(float DeltaSeconds) override;
};
