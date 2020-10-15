// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaActor.h"

/**
 * Applies damage to the actors health if they can currently be damaged. Fires OnDamage blueprint event.
 * @param amount The amount of damage to apply.
 */
void AArenaActor::Damage(int amount)
{
	if (CanDamage)
	{
		Health -= amount;
	}

	OnDamage(amount);
}

/**
* Plays the given flipbook
* @param flipbook The flipbook that should be played.
* @oaram loop Whether or not the flipbook should loop once finished.
*/
void AArenaActor::PlayFlipbook(UPaperFlipbook* flipbook, bool loop)
{
	GetSprite()->SetLooping(loop);
	GetSprite()->Play();
	GetSprite()->SetFlipbook(flipbook);
}
