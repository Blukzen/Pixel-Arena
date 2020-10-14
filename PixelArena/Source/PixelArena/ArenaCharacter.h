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
		UPROPERTY(EditAnywhere, Category = "Arena Character") float MoveSpeed = 400; // The characters move/walk speed.
		UPROPERTY(EditAnywhere, Category = "Arena Character") float AbilityCooldown = 200; // The length of the characters ability cooldown in ms.
		UPROPERTY(VisibleAnywhere, Category = "Arena Character|State", BlueprintReadOnly) TEnumAsByte<CharacterState> CharacterState = Idle; // Characters current state.
		UPROPERTY(VisibleAnywhere, Category = "Arena Character|State", BlueprintReadOnly) TEnumAsByte<Direction> Facing = South; // The direction the character is current facing.
		UPROPERTY(EditAnywhere, Category = "Arena Character|Animations") TMap<TEnumAsByte<Direction>, UPaperFlipbook*> IdleAnimations; // Idle animations for the different directions.
		UPROPERTY(EditAnywhere, Category = "Arena Character|Animations") TMap<TEnumAsByte<Direction>, UPaperFlipbook*> WalkingAnimations; // Walking animations for the different directions.
		UPROPERTY(BlueprintReadWrite, Category = "Arena Character|Hit Boxes") TMap<TEnumAsByte<Direction>, UBoxComponent*> AttackBoxes; // The attack boxes for the different directions.

		// Functions
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void Move();
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void SetVelocity(float speed, Direction direction);
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void FinishAttack();
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void FinishAbility();
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void SetHitbox(TEnumAsByte<Direction> direction, UBoxComponent* hitbox);
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void BeginAttack(TEnumAsByte<Direction> direction);
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void Attack(AArenaActor* other, int damageModifier);
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void ResetInput();
		UFUNCTION(BlueprintCallable, Category = "Arena Character") void ResetCooldown();

		// Events
		UFUNCTION(BlueprintImplementableEvent, Category="Arena Character") void IdleState(); // Idle state event.
		UFUNCTION(BlueprintImplementableEvent, Category="Arena Character") void WalkingState(); // Walking state event.
		UFUNCTION(BlueprintImplementableEvent, Category="Arena Character") void AttackState(float AttackTime, bool AttackDown); // Attack state event.
		UFUNCTION(BlueprintImplementableEvent, Category="Arena Character") void AbilityStart(); // Ability started event.
		UFUNCTION(BlueprintImplementableEvent, Category="Arena Character") void AbilityState(float AbilityTime, bool AbilityDown); // Ability state event.
		UFUNCTION(BlueprintImplementableEvent, Category="Arena Character") void AbilityEnd(); // Ability finished event.
		UFUNCTION(BlueprintImplementableEvent, Category="Arena Character") void AnimationFinished(); // Animation finished event.

	protected:
		// Properties
		UPROPERTY(VisibleAnywhere, Category="Arena Character|State") bool bIsMoving = false; // Is the character moving.
		UPROPERTY(VisibleAnywhere, Category="Arena Character|State") bool bIsAttacking = false; // Is the character attacking.
		UPROPERTY(VisibleAnywhere, Category="Arena Character|State") bool bIsAbility = false; // Is the character using an ability.
		UPROPERTY(VisibleAnywhere, Category="Arena Character|State") TMap<TEnumAsByte<Direction>, FDateTime> MoveInputMap; // Input map for movement, keeps track of when button were pressed.

		Direction MoveDirection = South; // The characters current movement direction.
		FDateTime attackDownTime = -1; // The time the attack key was pressed.
		bool attackKeyDown = false; // Whether the attack key is down.
		bool attackStarted = false; // Whether the attack has started yet.
		FDateTime abilityDownTime = -1; // The time the ability key was pressed.
		FDateTime abilityCooldownTime = -1; // The time the ability was last used.
		bool abilityKeyDown = false; // Whether the ability key is down.
		FVector& Velocity = GetCharacterMovement()->Velocity; // Reference to character movement velocity.

		// Functions
		void UpdateFacing();
		virtual void Tick( float DeltaSeconds ) override;
		virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

		// Input delegates
		void UpdateMovementInput(Direction direction, bool keyDown); DECLARE_DELEGATE_TwoParams(UpdateMovementInputDelegate, Direction, bool);
		void UpdateAttackInput(bool active); DECLARE_DELEGATE_OneParam(UpdateAttackInputDelegate, bool);
		void UpdateAbilityInput(bool active); DECLARE_DELEGATE_OneParam(UpdateAbilityInputDelegate, bool);
};