// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUserWidget.generated.h"

class UInventoryWidgetController;
/**
 * 
 */
UCLASS()
class BLACKOUT_API UInventoryUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetInventoryWidgetController(UInventoryWidgetController* InInventoryWidgetController);
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void InventoryWidgetControllerSet();
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
};
