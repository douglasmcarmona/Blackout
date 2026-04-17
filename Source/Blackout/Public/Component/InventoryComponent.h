#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventorySlot;

/**
 * Stores arbitrary data for a given item in an inventory slot, so that it can maintain its state when being stored or withdrawn
 */
USTRUCT(BlueprintType)
struct FSlotData
{
	GENERATED_BODY()

	/**
	 * Stores any arbitrary integer value an item needs 
	 */
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, int32> IntegerValues;
	
	/**
	 * Stores any arbitrary float value an item needs 
	 */
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, float> FloatValues;
	
	/**
	 * Stores any arbitrary boolean value an item needs 
	 */
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, bool> BoolValues;
};

/**
 * Represents an item in an inventory slot. Contains relevant data to persist the state of the item before storage 
 */
USTRUCT(BlueprintType)
struct FSlot
{
	GENERATED_BODY()

	/**
	 * The inventory index, used to retrieve this slot later
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 SlotNumber = -1;

	/**
	 * The class of the item. Used to spawn an instance of the item upon withdrawing
	 */
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<AActor> SlotItemClass = nullptr;

	/**
	 * Visual representation of the item while in the inventory
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTexture2D> SlotIcon = nullptr;

	/**
	 * Data structure containing arbitrary data
	 */
	UPROPERTY(BlueprintReadWrite)
	FSlotData SlotData;
	
	bool operator==(const FSlot& Other) const
	{
		return SlotNumber == Other.SlotNumber;
	}	
};

/**
 * Calls bound functions when an item is stored in the inventory
 * @param Slot(FSlot&) The slot created which represents the item
 * @param bIsRightHand(bool) True if the stored item was in player's right hand. False if it was the left hand instead
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemStored, const FSlot&, const bool);

/**
 * Calls bound functions when an item gets withdrawn from the inventory
 * @param SlotNumber(int32) The slot which the withdrawn item was in
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemWithdrawn, const int32)

/**
 * Calls bound functions if the item stored at the given time was the flashlight
 */
DECLARE_MULTICAST_DELEGATE(FOnFlashlightStored)

/**
 * Represents the implementation of the inventory mechanic. Capable of storing a non-runtime-changeable limited amount
 * of items
 */
UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class BLACKOUT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	/**
	 * Stores an item in the inventory, if it's actually storable. 
	 * @param SlotNumber The slot which the inventory will try and place the item into  
	 * @param bIsRightHand True if the item to be stored is in player's right hand. False if it's the one in the left hand
	 */
	void StoreItem(int32 SlotNumber, const bool bIsRightHand);

	/**
	 * Tries and withdraws an item from the given slot
	 * @param SlotNumber The slot the wished item is in
	 * @param bIsRightHand True if the withdrawn item must be placed into the player's right hand. False if it must be the
	 * left hand instead
	 */
	void WithdrawItem(const int32 SlotNumber, const bool bIsRightHand);

	/**
	 * Stores an item back in the inventory while loading itself after a level switch
	 * @param SlotNumber The inventory slot the restored item should take
	 * @param ItemName Used to get the item's icon and class
	 * @param IntegerMap Arbitrary integer data 
	 * @param FloatMap Arbitrary float data
	 * @param BoolMap Arbitrary boolean data
	 */
	void RestoreItem(const int32 SlotNumber, const FString& ItemName, const TMap<FString, int32>& IntegerMap, const TMap<FString, float>& FloatMap, const TMap<FString, bool>& BoolMap);
	/**
	 * Checks if the given slot is not already taken by an item. A storage attempt on a taken slot results in failure
	 * @param SlotNumber The slot to be checked for availability
	 * @param bIsFlashlight True if the checked slot is actually the flashlight slot. False otherwise
	 * @return True if the slot is not taken. False otherwise
	 */	
	bool IsSlotAvailable(const int32 SlotNumber, const bool bIsFlashlight);

	/**
	 * Retrieves the given slot from the inventory 
	 * @param SlotNumber The slot to be looked for
	 * @return A pointer to the FSlot in the SlotNumber position. Returns nullptr if the slot is not taken
	 */
	FSlot* GetSlot(const int32 SlotNumber);

	/**
	 * The amount of items the inventory can hold
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 InventorySize = 5;
	
	FOnItemStored OnItemStored;
	FOnItemWithdrawn OnItemWithdrawn;
	FOnFlashlightStored OnFlashlightStored;
	
private:
	/**
	 * The actual representation of the inventory in the game
	 */
	UPROPERTY(VisibleAnywhere)
	TArray<FSlot> Inventory;
};
