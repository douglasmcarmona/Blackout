// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlackoutGameModeBase.generated.h"

class UPaperNoteInfo;
class UInventoryItemInfo;

/**
 * A lighter version of our custom GameMode, with level transition mechanic stripped off 
 */
UCLASS()
class BLACKOUT_API ABlackoutGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	/**
	 * The PaperNoteInfo data asset reference. Each paper note actor in the game is identified by a number. That number
	 * is then used as an index to retrieve the text which will be written into them when it's initialized. 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPaperNoteInfo> PaperNoteInformation;

	/**
	 * The InventoryItemInfo data asset. Upon switching levels, the player's inventory must be persisted to keep gameplay
	 * consistency. This data asset is used to help restore uobject properties of items, which become invalid when switching
	 * levels. Examples of those properties include inventory icons and object classes
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInventoryItemInfo> InventoryItemInformation;

	/**
	 * Gather all levels in the game, which can then be retrieved by name
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;
};
