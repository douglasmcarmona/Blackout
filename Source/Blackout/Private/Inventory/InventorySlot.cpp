// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySlot.h"

UInventorySlot::UInventorySlot()
{
	SlotNumber = -1;
	SlotIcon = nullptr;
	SlotItemClass = nullptr;
}

int32 UInventorySlot::GetSlotNumber() const
{
	return SlotNumber;
}

void UInventorySlot::SetSlotNumber(const int32 InSlotNumber)
{
	SlotNumber = InSlotNumber;
}

TSubclassOf<AInteractableActor> UInventorySlot::GetSlotItemClass() const
{
	return SlotItemClass;
}

void UInventorySlot::SetSlotItemClass(UClass* InSlotItemClass)
{
	SlotItemClass = InSlotItemClass;
}

UTexture2D* UInventorySlot::GetSlotIcon() const
{
	return SlotIcon;
}

void UInventorySlot::SetSlotIcon(UTexture2D* InSlotIcon)
{
	SlotIcon = InSlotIcon;
}
