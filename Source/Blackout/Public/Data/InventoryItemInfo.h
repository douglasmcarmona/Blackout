#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryItemInfo.generated.h"

/**
 * Holds the required properties used by an actor to manage its presence between the inventory and the level 
 */
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	/**
	 * The name of the item. Identifies its type
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemName;

	/**
	 * Represents what the item looks like while in the inventory. 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> ItemIcon;

	/**
	 * Used by the spawn functions to properly turn the item back into an actor in the world
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> ItemClass;
};

/**
 * Interactable actors in the game must have properties that allow them to be spawned again after turning into items in the
 * inventory. Such properties are handled as allocated memory, which gets completely changed when the player travels to another level.
 * This data asset represents a stable source which can be researched to retrieve those properties for each and every actor,
 * allowing them to function correctly as inventory items, regardless of the current active level. 
 */
UCLASS()
class BLACKOUT_API UInventoryItemInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	 * Tries and gets the data regarding an item type 
	 * @param InventoryItemName The property to be used as look up index 
	 * @return A pointer to the element of the collection that has the given name. Returns null if there's no such element
	 */
	FInventoryItem* GetInventoryItemByName(const FString& InventoryItemName);

	/**
	 * Tries and gets the data regarding an item type 
	 * @param InventoryItemClass The property to be used as look up index
	 * @return A pointer to the element of the collection that has the given class. Returns null if there's no such element
	 */
	FInventoryItem* GetInventoryItemByClass(UClass* InventoryItemClass);
	
	// UObject override
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	
	
private:
	// The collection of all inventory item types in the game 
	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryItem> InventoryItems;	
};
