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

	/**
	 * Controls visibility of the pause button widget. Should be visible while traveling through the map and invisible
	 * when the pause input action is triggered
	 * @param bVisible True if the widget must be visible. False otherwise
	 */	
	void TogglePauseButton(const bool bVisible);

	/**
	 * Controls visibility of the pause menu widget. Should be visible when the pause action is triggered and invisible
	 * when it's triggered a second time or when the 'Resume' button is clicked on the menu
	 * @param bVisible True if the widget must be visible. False otherwise
	 */
	void TogglePauseMenu(const bool bVisible); 
	

protected:
	
	virtual void BeginPlay() override;
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

	/**
	 * Used to create the PauseButton widget object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> PauseButtonWidgetClass;

	/**
	 * Used to create the PauseMenu widget object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;	

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

	/**
	 * Reference to the instantiated PauseMenuWidget
	 */
	UPROPERTY()
	TObjectPtr<UUserWidget> PauseMenuWidget;

	/**
	 * Reference to the instantiated PauseButtonWidget
	 */
	UPROPERTY()
	TObjectPtr<UUserWidget> PauseButtonWidget;
};
