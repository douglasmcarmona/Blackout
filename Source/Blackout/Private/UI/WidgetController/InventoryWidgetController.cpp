// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "Component/InventoryComponent.h"

void UInventoryWidgetController::StoreItem(const int32 SlotNumber, const bool bIsRightHand)
{
	InventoryComponent->StoreItem(SlotNumber, bIsRightHand);
}

void UInventoryWidgetController::WithdrawItem(const int32 SlotNumber, const bool bIsRightHand)
{
	InventoryComponent->WithdrawItem(SlotNumber, bIsRightHand);
}

UInventoryComponent* UInventoryWidgetController::GetInventoryComponent() const
{
	return InventoryComponent;
}

void UInventoryWidgetController::SetInventoryComponent(UInventoryComponent* InInventoryComponent)
{
	InventoryComponent = InInventoryComponent;
	InventoryComponent->OnItemStored.AddLambda([this](const FSlot& InSlot, const bool bIsRightHand)
	{
		OnItemStoredDelegate.Broadcast(InSlot.SlotNumber, InSlot.SlotIcon);		
	});
	InventoryComponent->OnItemWithdrew.AddLambda([this](const int32 SlotNumber)
	{
		OnItemWithdrewDelegate.Broadcast(SlotNumber);
	});
}

void UInventoryWidgetController::LoadInventory()
{
	for (int32 i=0; i<InventoryComponent->InventorySize; i++)
	{		
		OnInventoryOpenDelegate.Broadcast(i, InventoryComponent->GetSlot(i));	
	}
}
