// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "Component/InventoryComponent.h"
#include "Inventory/InventorySlot.h"

void UInventoryWidgetController::StoreItem(const int32 SlotNumber, const bool bIsRightHand)
{
	InventoryComponent->StoreItem(SlotNumber, bIsRightHand);
}

UInventoryComponent* UInventoryWidgetController::GetInventoryComponent() const
{
	return InventoryComponent;
}

void UInventoryWidgetController::SetInventoryComponent(UInventoryComponent* InInventoryComponent)
{
	InventoryComponent = InInventoryComponent;
	InventoryComponent->OnItemStored.AddLambda([this](const UInventorySlot* InSlot, const bool bIsRightHand)
	{
		OnItemStoredDelegate.Broadcast(InSlot->GetSlotNumber(), InSlot->GetSlotIcon());		
	});
}

void UInventoryWidgetController::LoadInventory()
{
	for (int32 i=0; i<InventoryComponent->InventorySize; i++)
	{		
		OnInventoryOpenDelegate.Broadcast(i, InventoryComponent->GetSlot(i));	
	}
}
