// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

static FDateTime InputReleaseTime = -1; // The time used to tell weather a key has been released

/**
 * ArenaCharacter constructor
 * - Sets up input map
 * - Registers onfinished playing event
 */
AArenaCharacter::AArenaCharacter()
{
    // Setup Movement Input Map
    MoveInputMap.Add(North, InputReleaseTime);
    MoveInputMap.Add(East, InputReleaseTime);
    MoveInputMap.Add(South, InputReleaseTime);
    MoveInputMap.Add(West, InputReleaseTime);

    GetSprite()->OnFinishedPlaying.AddDynamic(this, &AArenaCharacter::AnimationFinished);
}

/**
 * Applies velocity based on the characters current MoveDirection.
 */
void AArenaCharacter::Move()
{
    switch (MoveDirection)
    {
        case North:
            Velocity = FVector(0, 0, MoveSpeed);
            break;
        case East:
            Velocity = FVector(MoveSpeed, 0, 0);
            break;
        case South:
            Velocity = FVector(0, 0, -MoveSpeed);
            break;
        case West:
            Velocity = FVector(-MoveSpeed, 0, 0);
        default:
            break;
    }
}

/**
 * TODO: Implement apply velocity
 * Does this have any use cases?
 */
void AArenaCharacter::ApplyVelocity(float speed, Direction direction)
{
}

/**
 * Sets the characters current velocity to the given speed and applies it in the given direction.
 * @param speed Speed to apply.
 * @param direction Direction to apply speed.
 */
void AArenaCharacter::SetVelocity(float speed, Direction direction)
{
    switch (direction)
    {
    case North:
        Velocity = FVector(0, 0, speed);
        break;
    case East:
        Velocity = FVector(speed, 0, 0);
        break;
    case South:
        Velocity = FVector(0, 0, -speed);
        break;
    case West:
        Velocity = FVector(-speed, 0, 0);
    default:
        break;
    }
}

/*
* Blueprint callable function to activate hitbox/begin attack.
*/
void AArenaCharacter::BeginAttack(TEnumAsByte<Direction> direction) {
    AttackBoxes[Facing]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    attackStarted = true;
}

/**
 * Blueprint event to fire once an attack has finished.
 */
void AArenaCharacter::FinishAttack()
{
    bIsAttacking = false;
    attackDownTime = -1;
    attackStarted = false;

    AttackBoxes[Facing]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

/**
 * Blueprint event to fire once an ability has finished.
 */
void AArenaCharacter::FinishAbility()
{
    bIsAbility = false;
    abilityDownTime = -1;
}

/*
* Blueprint callable function to damage another arena actor.
* @param other The arena actor to damage.
* @param damageModifier Attack damage dealt is multiplied by this.
*/
void AArenaCharacter::Attack(AArenaActor* other, int damageModifier) {
    // Don't attack self
    if (other == this)
        return;
    
    other->Damage(AttackDamage * FGenericPlatformMath::Pow(2, damageModifier));
}
/**
 * Resets the character input values to their defaults
 * TODO: Should use input map rather than strings for IsInputKeyDown
 */
void AArenaCharacter::ResetInput()
{
    const APlayerController* controller = GetWorld()->GetFirstPlayerController();

    UpdateMovementInput(North, controller->IsInputKeyDown(FKey("W")));
    UpdateMovementInput(West, controller->IsInputKeyDown(FKey("A")));
    UpdateMovementInput(South, controller->IsInputKeyDown(FKey("S")));
    UpdateMovementInput(East, controller->IsInputKeyDown(FKey("D")));

    UpdateAbilityInput(false);
    UpdateAttackInput(false);
}

/**
 * Updates the movement map and decides whether the character is moving or not
 * @param direction Direction input to update.
 * @param keyDown Whether or not this directional key has been pressed.
 */
void AArenaCharacter::UpdateMovementInput(Direction direction, bool keyDown)
{
    // Update map
    MoveInputMap[direction] = keyDown ? FDateTime::Now() : InputReleaseTime;

    if (MoveInputMap[North] > InputReleaseTime ||
        MoveInputMap[West] > InputReleaseTime ||
        MoveInputMap[South] > InputReleaseTime ||
        MoveInputMap[East] > InputReleaseTime)
    {
        bIsMoving = true;
    }
    else
    {
        bIsMoving = false;
    }
}

/**
 * Updates the characters facing direction.
 */
void AArenaCharacter::UpdateFacing()
{   
    // Find key that's down, that was pressed the most recent
    TPair<TEnumAsByte<Direction>, FDateTime> recent;
    for (const TPair<TEnumAsByte<Direction>, FDateTime>& pair : MoveInputMap)
    {
        if (pair.Value > recent.Value)
            recent = pair;
    }

    // Only update direction if we're actually moving
    if (bIsMoving)
    {
        // TODO: Replace with Facing, Separate direction no longer needed
        MoveDirection = recent.Key;
        Facing = recent.Key;
    }
}

/**
 * Attack input callback.
 */
void AArenaCharacter::UpdateAttackInput(bool active)
{
    attackKeyDown = active;

    if (!bIsAttacking && active)
    {
        bIsAttacking = true;
        attackDownTime = FDateTime::Now();
    }
}

/**
 * Ability input callback.
 */
void AArenaCharacter::UpdateAbilityInput(bool active)
{
    abilityKeyDown = active;

    // Check cooldown or if we are already in ability state
    if ((FDateTime::Now() - abilityCooldownTime).GetDuration().GetTotalMilliseconds() < AbilityCooldown || bIsAbility)
    {
        return;
    }
    
    if (active)
    {
        AbilityStart();
        bIsAbility = true;
        abilityDownTime = FDateTime::Now();
        abilityCooldownTime = FDateTime::Now();
    }
}

/*
* Sets the hitbox for the given direction
* @param direction The direction the hitbox is for
* @param hitbox The box component that makes the hitbox
*/
void AArenaCharacter::SetHitbox(TEnumAsByte<Direction> direction, UBoxComponent* hitbox) 
{
    hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    AttackBoxes.Add(direction, hitbox);
}

/**
 * Main character state loop
 */
void AArenaCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    switch (CharacterState)
    {
        case Idle:
            UpdateFacing();
            IdleState();
            if (IdleAnimations.Contains(Facing))
            {
                PlayFlipbook(IdleAnimations[Facing], true);
            }
            if (bIsMoving)
            {
                CharacterState = Walking;
                break;
            }
            if (bIsAttacking)
            {
                CharacterState = Attacking;
                break;
            }
            if (bIsAbility)
            {
                CharacterState = Ability;
                break;
            }
            break;
        case Walking:
            UpdateFacing();
            WalkingState();
            if (WalkingAnimations.Contains(Facing))
            {
                PlayFlipbook(WalkingAnimations[Facing], true);
            }
            if (bIsAttacking)
            {
                CharacterState = Attacking;
                break;
            }
            if (bIsAbility)
            {
                CharacterState = Ability;
                break;
            }
            if (!bIsMoving)
            {
                SetVelocity(0, Facing);
                CharacterState = Idle;
                break;
            }
            break;
        case Attacking:
            if (!bIsAttacking && bIsMoving)
            {
                CharacterState = Walking;
                break;
            }
            if (!bIsAttacking)
            {
                CharacterState = Idle;
                break;
            }
            
            if (!attackStarted)
                AttackState((FDateTime::Now() - attackDownTime).GetTotalMilliseconds(), attackKeyDown);
            break;
        case Ability:
            if (!bIsAbility && bIsMoving)
            {
                AbilityEnd();
                CharacterState = Walking;
                break;
            }
            if (!bIsAbility)
            {
                AbilityEnd();
                CharacterState = Idle;
                break;
            }
            AbilityState((FDateTime::Now() - abilityDownTime).GetTotalMilliseconds(), abilityKeyDown);
            break;
        default:
            break;
    }
}

/**
 * Binds character input callbacks
 */
void AArenaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Bind Movement Inputs
    PlayerInputComponent->BindAction<UpdateMovementInputDelegate>("North", IE_Pressed, this, &AArenaCharacter::UpdateMovementInput, North, true);
    PlayerInputComponent->BindAction<UpdateMovementInputDelegate>("North", IE_Released, this, &AArenaCharacter::UpdateMovementInput, North, false);
    PlayerInputComponent->BindAction<UpdateMovementInputDelegate>("East", IE_Pressed, this, &AArenaCharacter::UpdateMovementInput, East, true);
    PlayerInputComponent->BindAction<UpdateMovementInputDelegate>("East", IE_Released, this, &AArenaCharacter::UpdateMovementInput, East, false);
    PlayerInputComponent->BindAction<UpdateMovementInputDelegate>("South", IE_Pressed, this, &AArenaCharacter::UpdateMovementInput, South, true);
    PlayerInputComponent->BindAction<UpdateMovementInputDelegate>("South", IE_Released, this, &AArenaCharacter::UpdateMovementInput, South, false);
    PlayerInputComponent->BindAction<UpdateMovementInputDelegate>("West", IE_Pressed, this, &AArenaCharacter::UpdateMovementInput, West, true);
    PlayerInputComponent->BindAction<UpdateMovementInputDelegate>("West", IE_Released, this, &AArenaCharacter::UpdateMovementInput, West, false);

    // Bind Attack Inputs
    PlayerInputComponent->BindAction<UpdateAttackInputDelegate>("Attack", IE_Pressed, this, &AArenaCharacter::UpdateAttackInput, true);
    PlayerInputComponent->BindAction<UpdateAttackInputDelegate>("Attack", IE_Released, this, &AArenaCharacter::UpdateAttackInput, false);

    // Bind Ability Inputs
    PlayerInputComponent->BindAction<UpdateAttackInputDelegate>("Ability", IE_Pressed, this, &AArenaCharacter::UpdateAbilityInput, true);
    PlayerInputComponent->BindAction<UpdateAttackInputDelegate>("Ability", IE_Released, this, &AArenaCharacter::UpdateAbilityInput, false);
}