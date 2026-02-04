// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"

#include "Interaction/HandInterface.h"
#include "Interaction/InteractionInterface.h"
#include "Inventory/InventorySlot.h"

UInventoryComponent::UInventoryComponent()
{	
	PrimaryComponentTick.bCanEverTick = false;
	Inventory = TArray<UInventorySlot*>();
	Inventory.SetNum(InventorySize);
}

void UInventoryComponent::StoreItem(int32 SlotNumber, const bool bIsRightHand)
{
	AActor* StoredItem = bIsRightHand ? IHandInterface::Execute_GetRightHandItem(GetOwner()) : StoredItem = IHandInterface::Execute_GetLeftHandItem(GetOwner());
	if (!StoredItem) return;
	if (!IInteractionInterface::Execute_IsStorable(StoredItem)) return;
	
	const bool bIsFlashlight = IInteractionInterface::Execute_IsFlashLight(StoredItem); 
	SlotNumber = bIsFlashlight ? 0 : SlotNumber;
	if (!IsSlotAvailable(SlotNumber, bIsFlashlight)) return;
	Inventory[SlotNumber]->SetSlotItemClass(StoredItem->GetClass());
	Inventory[SlotNumber]->SetSlotIcon(IInteractionInterface::Execute_GetIcon(StoredItem));	
	OnItemStored.Broadcast(Inventory[SlotNumber], bIsRightHand);
}

bool UInventoryComponent::IsSlotAvailable(const int32 SlotNumber, const bool bIsFlashlight)
{
	if (Inventory[SlotNumber]->GetSlotIcon() != nullptr) return false;
	const bool IsSlotZero = SlotNumber == 0;	
	return !(bIsFlashlight ^ IsSlotZero);
}

UInventorySlot* UInventoryComponent::GetSlot(const int32 SlotNumber)
{
	return Inventory[SlotNumber];
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	for (int32 i = 0; i < InventorySize; i++)
	{		
		Inventory[i] = NewObject<UInventorySlot>();
		Inventory[i]->SetSlotNumber(i);
	}
}
