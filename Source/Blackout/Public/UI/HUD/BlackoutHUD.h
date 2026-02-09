// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlackoutHUD.generated.h"

class UInventoryUserWidget;
class UInventoryComponent;
class UInventoryWidgetController;
/**
 * 
 */
UCLASS()
class BLACKOUT_API ABlackoutHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	UInventoryWidgetController* GetInventoryWidgetController(UInventoryComponent* InventoryComponent);

	void InitInventoryWidget(UInventoryComponent* InventoryComponent);
	void CloseInventoryWidget() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UInventoryUserWidget> InventoryWidgetClass;	

private:
	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	UPROPERTY()
	TObjectPtr<UInventoryUserWidget> InventoryWidget;	
};
