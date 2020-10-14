// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ArenaCharacter.h"
#include "Engine/GameInstance.h"
#include "PixelArenaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIXELARENA_API UPixelArenaGameInstance : public UGameInstance
{
	GENERATED_BODY()

	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pixel Arena")
		TSubclassOf<class AArenaCharacter> CharacterClass; // Current selected class
	
};
