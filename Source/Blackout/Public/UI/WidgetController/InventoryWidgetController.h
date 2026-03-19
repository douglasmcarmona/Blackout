#pragma once

#include "CoreMinimal.h"
#include "InventoryWidgetController.generated.h"

class UInventorySlot;

/** 
 * Calls bound functions when the inventory gets open. Broadcasts slot data to the widget
 * @param SlotNumber(int32) The number of the current slot being processed
 * @param bIsTaken(bool) True if there's an item in this slot. False otherwise
 * @param Slot(FSlot&) The actual slot data struct
 * @param FlashlightBatteryPercent(float) Represents the value of the flashlight's current battery level. Used only by the slot
 * zero to show this value on itself
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnInventoryOpen, int32, SlotNumber, bool, bIsTaken, const FSlot&, Slot, float, FlashlightBatteryPercent);

/**
 * Calls bound functions when an item is stored in the inventory (in this case, the inventory component). Actually, this streams
 * the InventoryComponent's broadcasts to the widget
 * @param StoredItemSlotNumber(int32) The slot in which the item was stored
 * @param StoredItemIcon(UTextured2D) The visual representation of the item after being stored. This is how the player can
 * actually visualize the current status of the inventory
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemStoredSignature, int32, StoredItemSlotNumber, UTexture2D*,
                                             StoredItemIcon);

/**
 * Calls bound functions when an item is withdrawn from the inventory. In practice, this will indicate that the widget should
 * reset the slot to the empty state
 * @param SlotNumber(int32) the slot from which the item was withdrawn
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemWithdrewSignature, int32, SlotNumber);

/**
 * Calls bound functions when the flashlight gets its battery percentage changed. This sends updates to the widget when
 * the flashlight discharge mechanic is executed
 * @param BatteryPercentage(float) The new value of flashlight's battery percentage property
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFlashlightBatteryChangedSignature, float, BatteryPercentage);

class UInventoryComponent;

/**
 * The controller class that sets a communication bridge between the InventoryComponent and the InventoryWidget
 */
UCLASS(Blueprintable, BlueprintType)
class BLACKOUT_API UInventoryWidgetController : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Called by the widget to start the store item mechanic
	 * @param SlotNumber The slot selected from the inventory to store the item
	 * @param bIsRightHand True if the item to be stored is the one in the player's right hand. False if it's the one in their
	 * left hand instead
	 */
	UFUNCTION(BlueprintCallable)
	void StoreItem(const int32 SlotNumber, const bool bIsRightHand);

	/**
	 * Called by the widget to start the withdraw item mechanic
	 * @param SlotNumber The slot selected from the inventory to withdraw the item from
	 * @param bIsRightHand True if the item should be placed in the player's right hand. False if it should be placed in their
	 * left hand instead
	 */
	UFUNCTION(BlueprintCallable)
	void WithdrawItem(const int32 SlotNumber, const bool bIsRightHand);

	/**
	 * Return this controller's InventoryComponent reference
	 * @return The InventoryComponent reference
	 */
	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetInventoryComponent() const;

	/**
	 * Performs all predicted operations upon receiving a valid InventoryComponent value
	 * @param InInventoryComponent The InventoryComponent to be used by this class afterwards
	 */
	UFUNCTION(BlueprintCallable)
	void SetInventoryComponent(UInventoryComponent* InInventoryComponent);

	/**
	 * Broadcasts data related to all inventory slots to the widget. Called when the player opens the inventory
	 */
	UFUNCTION(BlueprintCallable)
	void LoadInventory() const;

	// UObject override
	virtual UWorld* GetWorld() const override;
	
	UPROPERTY(BlueprintAssignable)
	FOnItemStoredSignature OnItemStoredDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnItemWithdrewSignature OnItemWithdrewDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryOpen OnInventoryOpenDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnFlashlightBatteryChangedSignature OnFlashlightBatteryChangedDelegate;

private:
	/**
	 * Performs the mechanic that depletes the flashlight's battery in case it was stored while turned on, using values set
	 * on the flashlight actor, which are later saved in the FSlotData maps when the flashlight is stored.
	 */
	UFUNCTION()
	void DischargeFlashlightBattery();

	/**
	 * This class' InventoryComponent reference
	 */
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/**
	 * Handles the flashlight battery discharge timer. It's valid only when the flashlight is stored while turned on and
	 * the battery has not reached zero yet
	 */
	FTimerHandle BatteryDischargeTimerHandle;
};
