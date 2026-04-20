#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

struct FSlotData;

UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Represents all kinds of operations implementing objects can perform if said objects would interact with the calling objects
 * (such as the player)
 * Main implementer: InteractableActor
 */
class BLACKOUT_API IInteractionInterface
{
	GENERATED_BODY()

	
public:
	/**
	 * Make the required settings to become highlighted in the world
	 * 
	 * The highlight mechanic works as a outline rendered on top of the object's mesh on demand 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction|Hightlight")
	void Highlight();

	/**
	 * Make the required settings to unhighlight itself in the world. The mesh's rendered outline gets unrendered
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction|Hightlight")
	void Unhighlight();

	/**
	 * Implementing objects must implement this to perform their corresponding interaction gameplay mechanic 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact();

	/**
	 * Allows calling objects to check if the implementing object can be picked up from the world
	 * @return True if the object can be picked up. False otherwise
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool IsPickable() const;

	/**
	 * Allows calling objects to check if the implementing object can be stored in the inventory
	 * @return True if the object can be stored. False otherwise
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool IsStorable() const;
	
	/**
	 * Allows calling objects to check if the implementing object can be thrown away while being held
	 * @return True if the object can be thrown away. False otherwise
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool IsThrowable() const;

	/**
	 * Performs the throw mechanic on implementing objects
	 * @param ThrowForce The force to be applied to the implementing object 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Throw(const FVector& ThrowForce);

	/**
	 * Performs required settings to allow implementing objects to be picked up by calling objects
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void PreparePickup();

	/**
	 * Allows calling objects to retrieve the icon property of implementing objects
	 * @return The icon that represents the object while in the inventory
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	UTexture2D* GetIcon();

	/**
	 * Tells implementing objects to fill up FSlotData maps with arbitrary data in order to maintain their state
	 * @param SlotData The data structure to be saved with the slot
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction|Inventory", meta = (ForceAsFunction))
	void HandleStoredItemSlotData(UPARAM(ref) FSlotData& SlotData);

	/**
	 * Tells implementing objects to read the arbitrary data from FSlotData maps in order to restore their state like
	 * it was before storage
	 * @param SlotData The data structure to be retrieved from the slot
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction|Inventory", meta = (ForceAsFunction))
	void HandleWithdrawnItemSlotData(const FSlotData& SlotData);

	/**
	 * Handy function to return PersistentGuid property value
	 * @return The unique identifier related to the implementing object
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction|Inventory")
	FGuid GetGuid();
};
