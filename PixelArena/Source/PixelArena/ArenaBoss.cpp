#include "ArenaBoss.h"

/**
 * Constructor
 */
AArenaBoss::AArenaBoss()
{
    // Add callback for flip book animation finished.
    GetSprite()->OnFinishedPlaying.AddDynamic(this, &AArenaBoss::AnimationFinished);
}

/**
 * Attack function template.
 */
void AArenaBoss::Attack() {}

/**
 * Ability function template.
 */
void AArenaBoss::Ability() {}

/**
 * Called every frame
 */
void AArenaBoss::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Fire blueprint event based on current state
    switch(CurrentState)
    {
        case BossIdle:
            IdleState();
            break;
        case BossAttacking:
            AttackState();
            break;
        case BossAbility:
            AbilityState();
            break;
        default:
            break;
    }
}
