#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BlackoutGameInstance.generated.h"

/**
 * Holds arbitrary data used by an inventory item in its functionality
 */
USTRUCT()
struct FInventorySlotMapData
{
	GENERATED_BODY()
	
	// Arbitrary integer values
	UPROPERTY()
	TMap<FString, int32> IntegerMap;
	
	// Arbitrary float values 
	UPROPERTY()
	TMap<FString, float> FloatMap;
	
	// Arbitrary boolean values
	UPROPERTY()
	TMap<FString, bool> BoolMap;
};

/**
 * Represents the data needed to restore an item from the inventory after switching levels
 */
USTRUCT()
struct FInventorySlotData
{
	GENERATED_BODY()
	
	// The item's unique identifier, obtained from the actor
	UPROPERTY()
	FGuid PersistentGuid;
	
	// The name of the item. Used to get uobject data from InventoryItemInfo data asset
	UPROPERTY()
	FString ItemName = FString();
	
	// The inventory slot the item was in and must be restored into
	UPROPERTY()
	int32 SlotNumber = -1;
	
	// Gathers the item's set of arbitrary data
	UPROPERTY()
	FInventorySlotMapData InventorySlotMapData;
};

/**
 * Tracks down and stored data regarding actors that still are in their original state (the same as the one at game start)
 * when the player transitions to another level
 */
USTRUCT()
struct FPlacedActorData
{
	GENERATED_BODY()
	
	// The transform value for the actor before the current level gets torn down
	UPROPERTY()
	FTransform ActorTransform;
	
	// Holds the actor instance after serialization
	UPROPERTY()
	TArray<uint8> Bytes;
	
	/**
	 * If this property is set to true, then the actor was destroyed before level transition, which means it must not
	 * exist when the level gets loaded a second time and therefore must be destroyed immediately right after being
	 * constructed
	 */	
	UPROPERTY()
	bool bIsPendingKill = false;
};

/**
 * Tracks down and stores data regarding all actors that were dynamically spawned in the level during gameplay and must
 * be active when the levels loads up a second time
 */
USTRUCT()
struct FSpawnedActorData
{
	GENERATED_BODY()
	
	// The unique identifier of the actor
	UPROPERTY()
	FGuid ActorGuid;
	
	// The class of the actor. Used to spawn it again in the level
	UPROPERTY()
	TSubclassOf<AActor> ActorClass;
	
	// The actor transform value before level teardown
	UPROPERTY()
	FTransform ActorTransform;
	
	// Holds the actor instance after serialization
	UPROPERTY()
	TArray<uint8> Bytes;
	
	bool operator==(const FSpawnedActorData& OtherActorData) const
	{
		return ActorGuid == OtherActorData.ActorGuid;
	}
};

/**
 * Stored all data needed to maintain the state of all interactable actors in a level, which must have consistency when
 * the player transitions back to that level a second time
 */
USTRUCT()
struct FLevelData
{
	GENERATED_BODY()

	/**
	 * Tracks down all actors that are part of the initial state of the level and stayed like that right before the
	 * transition
	 */
	UPROPERTY()
	TMap<FGuid, FPlacedActorData> PlacedActorsData = TMap<FGuid, FPlacedActorData>();

	/**
	 * Tracks down all actors that were placed in the level at runtime and were not destroyed before the transition
	 */
	UPROPERTY()
	TArray<FSpawnedActorData> SpawnedActorsData = TArray<FSpawnedActorData>();

	/**
	 * Controls specific functionality to be executed when the level is being loaded for the first time or by 
	 * subsequent transitions 
	 */
	UPROPERTY()
	bool bIsFirstLoad = true;
};

/**
 * Notifies bound functions that the music general control has been changed
 * @param bMusicEnabled True if the music is now enabled. False if it has been disabled
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMusicToggledSignature, const bool, bMusicEnabled);

/**
 * Notifies bound functions that the sound effects general control has been changed
 * @param bSFXEnabled True if sound effects are now enabled. False if they have been disabled
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSFXToggledSignature, const bool, bSFXEnabled);
/**
 * Out custom GameInstance object. Used to persist data while switching levels, such as the player's inventory and changes
 * in the level regarding dynamic actors (such as interactable actors).
 */
UCLASS()
class BLACKOUT_API UBlackoutGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	/**
	 * Keeps track of an inventory item internally. Used later to restore the item into the inventory after switching levels
	 * @param PersistentGuid The unique identifier of the item, obtained from the actor which it represents
	 * @param SlotNumber The position (slot) of the item in the inventory
	 * @param ItemName The name of the item. Enables restoration of object data with the assist of the InventoryItem info
	 * Data asset, such as the class and inventory icon of the item
	 * @param IntegerMap Set of arbitrary integer values, identified by a string key
	 * @param FloatMap Set of arbitrary float values, identified by a string key
	 * @param BoolMap Set of arbitrary boolean values, identified by a string key
	 */
	void SaveInventorySlotData(const FGuid& PersistentGuid, const int32 SlotNumber, const FString& ItemName, const TMap<FString, int32>& IntegerMap, const TMap<FString, float>& FloatMap, const TMap<FString, bool>& BoolMap);

	/**
	 * Used to retrieve a previously saved inventory item data, which is then used to restore the slot in the inventory
	 * @param SlotNumber The position (slot) which the item must be put into
	 * @param PersistentGuid(output) The unique identifier of the item, obtained from the actor which it represents
	 * @param ItemName(output) The name of the item. Enables restoration of object data with the assist of the InventoryItem info
	 * Data asset, such as the class and inventory icon of the item
	 * @param IntegerMap(output) Set of arbitrary integer values, identified by a string key
	 * @param FloatMap(output) Set of arbitrary float values, identified by a string key
	 * @param BoolMap(output) Set of arbitrary boolean values, identified by a string key
	 * @return True if the item was retrieved successfully. False otherwise
	 */
	bool LoadInventorySlotData(const int32 SlotNumber, FGuid& PersistentGuid, FString& ItemName, TMap<FString, int32>& IntegerMap, TMap<FString, float>& FloatMap, TMap<FString, bool>& BoolMap);

	/**
	 * Empties the inventory
	 */
	void InventoryEmpty() { InventoryData.Empty(); }

	/**
	 * Tracks down an actor instance that is originally part of the level object
	 * @param MapName The name of the level the actor is in
	 * @param ActorGuid The actor's unique identifier. Used to get the actor later when the level loads again
	 * @param PlacedActor The data struct with all data needed to persist the state of the actor
	 * @return True if the struct was successfully inserted into the placed actors array. False otherwise
	 */
	bool AddToPlacedActors(const FString& MapName, const FGuid& ActorGuid, const FPlacedActorData& PlacedActor);

	/**
	 * Tracks down an instance of an actor spawned in the world (such as withdrawn from the inventory) which was not
	 * destroyed before the level is torn down
	 * @param MapName The name of the level the actor was spawned in
	 * @param SpawnedActor The data struct with all data needed to persist the state of the actor
	 * @return True if the struct was successfully inserted into the spawned actors array. False otherwise
	 */
	bool AddToSpawnedActors(const FString& MapName, const FSpawnedActorData& SpawnedActor);

	/**
	 * Checks the status of music control
	 * @return True if music is currently enabled. False otherwise
	 */
	bool IsMusicEnabled() const { return bIsMusicEnabled; }
	
	/**
	 * Checks the status of sound effects control
	 * @return True if sound effects are currently enabled. False otherwise
	 */
	bool IsSFXEnabled() const { return bIsSFXEnabled; }

	/**
	 * Flips music control value
	 */
	void ToggleMusic();
	
	/**
	 * Flips sound effects control value
	 */
	void ToggleSFX();
	
	
	// Saves the number of the slot where the item in player's right hand now is
	int32 RightHandItemInventorySlotNumber;
	
	// Saves the number of the slot where the item in player's left hand now is
	int32 LeftHandItemInventorySlotNumber;
	
	/* Gathers all data used by the game to persist the state of all interactable actors that exist in each one of
	levels so that level transitions can happen with the proper consistency */
	TMap<FString, FLevelData> LevelTransitionData;
	
	UPROPERTY(BlueprintAssignable)
	FOnMusicToggledSignature OnMusicToggledDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnSFXToggledSignature OnSFXToggledDelegate;
	
private:
	// The inventory's data-only representation
	TArray<FInventorySlotData> InventoryData;
	
	// Controls if music is currently enabled in the game
	bool bIsMusicEnabled = true;
	
	// Controls if sound effects are currently enabled in the game
	bool bIsSFXEnabled = true;
};
