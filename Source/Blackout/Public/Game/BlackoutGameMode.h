#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlackoutGameMode.generated.h"

class UInventoryItemInfo;
class UPaperNoteInfo;

/**
 * Out custom GameMode. Currently, it only stored the instance of the PaperNoteInfo data asset used to retrieve text
 * to be contained in those actors 
 */
UCLASS()
class BLACKOUT_API ABlackoutGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	/**
	 * Performs all required functionality to keep gameplay consistency while switching levels
	 * @param MapName The level which the player wll travel to
	 */
	UFUNCTION(BlueprintCallable)
	void TravelToMap(const FString& MapName);
	
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
	
protected:
	// Actor override
	virtual void BeginPlay() override;
};
