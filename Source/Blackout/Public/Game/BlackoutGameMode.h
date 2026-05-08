#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlackoutGameMode.generated.h"

class UInventoryItemInfo;
class UPaperNoteInfo;
struct FLevelData;

/**
 * Our custom GameMode. Currently, it only stored the instance of the PaperNoteInfo data asset used to retrieve text
 * to be contained in those actors 
 */
UCLASS()
class BLACKOUT_API ABlackoutGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	/**
	 * Performs all required functionality to keep gameplay consistency while switching levels
	 * @param MapName The level which the player will travel to
	 */
	UFUNCTION(BlueprintCallable)
	void TravelToMap(const FString& MapName);	

	// GameModeBase override
	virtual void StartPlay() override;

	/**
	 * When an InteractableActor placed in the world gets destroyed at runtime, it calls this function to mark itself as
	 * pending kill, which means that that actor will wait to be destroyed when the level starts  
	 * @param MapName The name of the level the actor was destroyed in
	 * @param PersistentGuid The identifier of the actor
	 */
	void MarkActorAsPendingKill(const FString& MapName, const FGuid& PersistentGuid) const;
	
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
	
private:
	/**
	 * Executed before level transition to persist the state of all InteractableActors that are originally contained in the
	 * level.
	 */
	void SavePlacedActors() const;

	/**
	 * Executed before level transition to persist the state of all InteractableActors that were spawned in a level throughout
	 * the course of the game and didn't get destroyed before the transition.
	 */
	void SaveSpawnedActors() const;
	
	/**
	 * Executed upon level start to restore the state of all InteractableActors that are originally part of a level  
	 * @param LevelData Data structure that contains relevant data used to restore all InteractableActors of a level
	 */
	void LoadPlacedActors(FLevelData& LevelData) const;

	/**
	 * Executed upon level start to restore the state of all InteractableActors that were spawned in a level
	 * and didn't get destroyed before its last teardown
	 * @param LevelData Data structure that contains relevant data used to restore all InteractableActors of a level
	 */
	void LoadSpawnedActors(FLevelData& LevelData) const;
};
