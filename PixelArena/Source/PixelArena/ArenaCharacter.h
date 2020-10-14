// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <map>


#include "ArenaActor.h"
#include "PaperCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"

#include "ArenaCharacter.generated.h"

UENUM(BlueprintType)
enum CharacterState
{
	Idle UMETA(DisplayName = "Idle"),
	Walking UMETA(DisplayName = "Walking"),
	Attacking UMETA(DisplayName = "Attacking"),
	Ability UMETA(DisplayName = "Ability")
};


UENUM(BlueprintType)
enum Direction {
	North UMETA(DisplayName = "North"),
    East UMETA(DisplayName = "East"),
    South UMETA(DisplayName = "South"),
    West UMETA(DisplayName = "West")
};

UCLASS()
class PIXELARENA_API AArenaCharacter : public AArenaActor
{
	GENERATED_BODY()
	
	public:
		// Constructor
		AArenaCharacter();

		// Properties
		UPROPERTY(EditAnywhere, Category="Arena Character") float MoveSpeed = 400;
		UPROPERTY(EditAnywhere, Category="Arena Character") float AbilityCooldown = 200;
		UPROPERTY(VisibleAnywhere, Category="Arena Character|State", BlueprintReadOnly) TEnumAsByte<CharacterState> CharacterState = Idle;
		UPROPERTY(VisibleAnywhere, Category="Arena Character|State", BlueprintReadOnly) TEnumAsByte<Direction> Facing = South;
		UPROPERTY(EditAnywhere, Category="Arena Character|Animations") TMap<TEnumAsByte<Direction>, UPaperFlipbook*> IdleAnimations;
		UPROPERTY(EditAnywhere, Category="Arena Character|Animations") TMap<TEnumAsByte<Direction>, UPaperFlipbook*> WalkingAnimations;
		UPROPERTY(BlueprintReadWrite, Category = "Arena Character|Hit Boxes") TMap<TEnumAsByte<Direction>, UBoxComponent*> AttackBoxes;

		// Functions
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void Move();
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void ApplyVelocity(float speed, Direction direction);
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void SetVelocity(float speed, Direction direction);
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void FinishAttack();
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void FinishAbility();
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void SetHitbox(TEnumAsByte<Direction> direction, UBoxComponent* hitbox);
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void BeginAttack(TEnumAsByte<Direction> direction);
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void Attack(AArenaActor* other, int damageModifier);
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void ResetInput();
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void ResetCooldown();

		// Events
		UFUNCTION(BlueprintImplementableEvent, Category="Arena Character") void IdleState();
		UFUNCTION(BlueprintImplementableEvent, Category="Arena Character") void WalkingState();
		UFUNCTION(BlueprintImplementableEvent, Category="Arena Character") void AttackState(float AttackTime, bool AttackDown);
		UFUNCTION(BlueprintImplementableEvent, Category="Arena Character") void AbilityStart();
		UFUNCTION(BlueprintImplementableEvent, Category="Arena Character") void AbilityState(float AbilityTime, bool AbilityDown);
		UFUNCTION(BlueprintImplementableEvent, Category="Arena Character") void AbilityEnd();
		UFUNCTION(BlueprintImplementableEvent, Category="Arena Character") void AnimationFinished();
		

	protected:
		// Properties
		UPROPERTY(VisibleAnywhere, Category="Arena Character|State") bool bIsMoving = false;
		UPROPERTY(VisibleAnywhere, Category="Arena Character|State") bool bIsAttacking = false;
		UPROPERTY(VisibleAnywhere, Category="Arena Character|State") bool bIsAbility = false;
		UPROPERTY(VisibleAnywhere, Category="Arena Character|State") TMap<TEnumAsByte<Direction>, FDateTime> MoveInputMap;

		Direction MoveDirection = South;
		FDateTime attackDownTime = -1;
		bool attackKeyDown = 0;
		bool attackStarted = false;
		FDateTime abilityDownTime = -1;
		FDateTime abilityCooldownTime = -1;
		bool abilityKeyDown = 0;
		FVector& Velocity = GetCharacterMovement()->Velocity;

		// Functions
		void UpdateFacing();
		virtual void Tick( float DeltaSeconds ) override;
		virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

		// Input delegates
		void UpdateMovementInput(Direction direction, bool keyDown); DECLARE_DELEGATE_TwoParams(UpdateMovementInputDelegate, Direction, bool);
		void UpdateAttackInput(bool active); DECLARE_DELEGATE_OneParam(UpdateAttackInputDelegate, bool);
		void UpdateAbilityInput(bool active); DECLARE_DELEGATE_OneParam(UpdateAbilityInputDelegate, bool);
};