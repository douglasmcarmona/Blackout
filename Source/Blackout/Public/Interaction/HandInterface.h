#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HandInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHandInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Provides a way to perform operations regarding the player's hands. Used mainly by the inventory while storing/withdrawing
 * items.
 * Main implementer: BlackoutCharacter 
 */
class BLACKOUT_API IHandInterface
{
	GENERATED_BODY()
	
public:
	/**
	 * Gets the item currently in the implementing object's right hand
	 * @return The reference to the item in that hand
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hand")
	AActor* GetRightHandItem() const;
	
	/**
	 * Gets the item currently in the implementing object's left hand
	 * @return The reference to the item in that hand
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hand")
	AActor* GetLeftHandItem() const;

	/**
	 * Places the given item into the implementing object's right hand  
	 * @param Item The item which will be placed 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hand")
	void SetRightHandItem(AActor* Item);

	/**
	 * Places the given item into the implementing object's left hand  
	 * @param Item The item which will be placed
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hand")
	void SetLeftHandItem(AActor* Item);

	/**
	 * Called to perform the required setting to lose the item in the left hand and make it interactable again
	 * @return The dropped item previously in the implementing object's left hand
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hand")
	AActor* DropLeftHandItem();

	/**
	 * Called to perform the required setting to lose the item in the right hand and make it interactable again
	 * @return The dropped item previously in the implementing object's right hand
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hand")
	AActor* DropRightHandItem();

	/**
	 * Allows calling objects to check if an implementing object's hand is currently holding an item 
	 * @param bIsRightHand True if the right hand should be checked. False if it's the left hand instead
	 * @return True if the hand is available. False if it's busy
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hand")
	bool IsHandHoldingItem(const bool bIsRightHand) const;

	/**
	 * Allows calling objects to get the world location of one of the implementing object's hand components
	 * @param bIsRightHand True if the location of the right hand should be get. False if it's the left hand's instead
	 * @return The world location of the desired hand
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hand")
	FVector GetHandLocation(const bool bIsRightHand) const;
	
};
