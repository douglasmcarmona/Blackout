#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlackoutHUD.generated.h"

class UInventoryUserWidget;
class UInventoryComponent;
class UInventoryWidgetController;

/**
 * Our custom HUD. Used mainly to control the inventory widget 
 */
UCLASS()
class BLACKOUT_API ABlackoutHUD : public AHUD
{
	GENERATED_BODY()

public:
	/**
	 * Gets a valid InventoryWidgetComponent reference. Used to create the inventory widget correctly
	 * @param InventoryComponent The inventory component the widget controller is dependent of
	 * @return The InventoryWidgetController to be used by the inventory widget
	 */
	UFUNCTION(BlueprintPure)
	UInventoryWidgetController* GetInventoryWidgetController(UInventoryComponent* InventoryComponent);

	/**
	 * Creates and renders the inventory widget on player's screen
	 * @param InventoryComponent The main dependency of the widget controller 
	 */
	void InitInventoryWidget(UInventoryComponent* InventoryComponent);

	/**
	 * Removes the inventory widget from the player's screen
	 */
	void CloseInventoryWidget() const;

protected:
	/**
	 * The class used to create the InventoryWidgetController object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;

	/**
	 * Used to create the InventoryWidget object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UInventoryUserWidget> InventoryWidgetClass;	

private:
	/**
	 * Reference to the instantiated InventoryWidgetController object
	 */
	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	/**
	 * Reference to the instantiated InventoryWidget object
	 */
	UPROPERTY()
	TObjectPtr<UInventoryUserWidget> InventoryWidget;	
};
