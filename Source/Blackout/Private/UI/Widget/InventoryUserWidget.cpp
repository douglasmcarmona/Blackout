// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/InventoryUserWidget.h"

void UInventoryUserWidget::SetInventoryWidgetController(UInventoryWidgetController* InInventoryWidgetController)
{
	InventoryWidgetController = InInventoryWidgetController;
	InventoryWidgetControllerSet();
}
