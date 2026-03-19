#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUserWidget.generated.h"

class UInventoryWidgetController;
/**
 * Represents the visual representation of the inventory in the game. Therefore, it's where the player will actually interact
 * with it, by storing and withdrawing items. Consequentially, this is the starting point of those mechanics, instead of
 * regular InputActions
 */
UCLASS()
class BLACKOUT_API UInventoryUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Receives a valid InventoryWidgetController reference to set its internal property
	 * @param InInventoryWidgetController The InventoryWidgetController value
	 */
	UFUNCTION(BlueprintCallable)
	void SetInventoryWidgetController(UInventoryWidgetController* InInventoryWidgetController);
	
protected:
	/**
	 * Fires off following mechanics which can be executed only after the WidgetController becomes valid 
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void InventoryWidgetControllerSet();

	/**
	 * The WidgetController reference used by this widget
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
};
