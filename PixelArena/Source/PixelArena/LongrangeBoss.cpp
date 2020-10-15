// Fill out your copyright notice in the Description page of Project Settings.


#include "LongrangeBoss.h"

/**
 * Teleports the boss to one of the teleport points based on whether he's at the center or not.
 */
void ALongrangeBoss::Teleport()
{
    if (bAtCenter)
        TeleportRandomCorner();
    else
        TeleportCenter();
}

/**
 * Spawns in basic projectile and fires towards player.
 */
void ALongrangeBoss::BasicAttack()
{
    AttackCount = AttackCount + 1;
    
    UWorld* const World = GetWorld();
    if (World)
    {
        AArenaProjectile* projectile = World->SpawnActor<AArenaProjectile>(BasicProjectile);
        projectile->SetActorLocation(GetActorLocation() + BasicProjectileOffset);
        projectile->FireAtPlayer();
    }
}


/**
 * Spawns in special projectile in a wave of 5 at varying angles.
 */
void ALongrangeBoss::SpecialAttack()
{
    AttackCount = 5;
    UWorld* const World = GetWorld();

    if (World)
    {
        FVector playerLocation = World->GetFirstPlayerController()->GetPawn()->GetActorLocation();
        FVector direction = playerLocation - GetActorLocation();

        direction = direction.RotateAngleAxis(-45, FVector(0, 1, 0));
        
        for (int i = 0; i < 5; i++)
        {
            AArenaProjectile* projectile = World->SpawnActor<AArenaProjectile>(SpecialProjectile);
            projectile->SetActorLocation(GetActorLocation() + SpecialProjectileOffset);
            projectile->Fire(direction);
            direction = direction.RotateAngleAxis(22.5, FVector(0, 1, 0));
        }
    }
}

/**
 * Teleports the boss to the center spawn.
 */
void ALongrangeBoss::TeleportCenter()
{
    bAtCenter = true;
    SetActorLocation(CenterSpawn->GetActorLocation());
}

/**
 * Teleports the boss to a random corner spawn.
 */
void ALongrangeBoss::TeleportRandomCorner()
{
    bAtCenter = false;
    SetActorLocation(CornerSpawns[FMath::RandRange(0, 3)]->GetActorLocation());
}

/**
 * Called every frame
 */
void ALongrangeBoss::Tick(float DeltaSeconds)
{
    // Logic based on bosses current state.
    switch(CurrentState)
    {
        // Boss idle state.
        case BossIdle:
            if (HitCount >= 5)
            {
                HitCount = 0;
                CurrentState = BossAbility;
                break;
            }
            if (bPlayerVisible && !bShouldTeleport)
            {
                CurrentState = BossAttacking;
                break;
            }
                
            IdleState();
            break;

        // Boss attacking state.
        case BossAttacking:
            if (HitCount >= 5)
            {
                HitCount = 0;
                CurrentState = BossAbility;
                break;
            }
            if (!bPlayerVisible)
            {
                CurrentState = BossIdle;
                break;
            }
            if (AttackCount >= 5)
            {
                AttackCount = 0;
                bShouldTeleport = true;
                CurrentState = BossIdle;
                break;
            }
                
            AttackState();
            break;

        // Boss ability state.
        case BossAbility:
            if (bTeleported)
            {
                CanDamage = true;
                bTeleported = false;
                bTeleporting = false;
                CurrentState = BossIdle;
                break;
            }
            
            AbilityState();
            break;
        default:
            break;
    }
}
