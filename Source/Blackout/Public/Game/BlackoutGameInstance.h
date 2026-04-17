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
 * Out custom GameInstance object. Used to persist data while switching levels, such as the player's inventory and changings
 * in the level regarding dynamic actors (such as interactable actors).
 */
UCLASS()
class BLACKOUT_API UBlackoutGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	/**
	 * Keeps track of a inventory item internally. Used later to restore the item into the inventory after switching levels
	 * @param SlotNumber The position (slot) of the item in the inventory
	 * @param ItemName The name of the item. Enables restoration of object data with the assist of the InventoryItem info
	 * Data asset, such as the class and inventory icon of the item
	 * @param IntegerMap Set of arbitrary integer values, identified by a string key
	 * @param FloatMap Set of arbitrary float values, identified by a string key
	 * @param BoolMap Set of arbitrary boolean values, identified by a string key
	 */
	void SaveInventorySlotData(const int32 SlotNumber, const FString& ItemName, const TMap<FString, int32>& IntegerMap, const TMap<FString, float>& FloatMap, const TMap<FString, bool>& BoolMap);

	/**
	 * Used to retrieve a previously saved inventory item data, which is then used to restore the slot in the inventory 
	 * @param SlotNumber The position (slot) which the item must be put into
	 * @param ItemName(output) The name of the item. Enables restoration of object data with the assist of the InventoryItem info
	 * Data asset, such as the class and inventory icon of the item
	 * @param IntegerMap(output) Set of arbitrary integer values, identified by a string key
	 * @param FloatMap(output) Set of arbitrary float values, identified by a string key
	 * @param BoolMap(output) Set of arbitrary boolean values, identified by a string key
	 * @return True if the item was retrieved successfully. False otherwise
	 */
	bool LoadInventorySlotData(const int32 SlotNumber, FString& ItemName, TMap<FString, int32>& IntegerMap, TMap<FString, float>& FloatMap, TMap<FString, bool>& BoolMap);
	void InventoryEmpty() { InventoryData.Empty(); }
	
private:
	// The inventory's data-only representation
	TArray<FInventorySlotData> InventoryData;
};
