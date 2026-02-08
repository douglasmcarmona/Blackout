// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"

#include "Interaction/HandInterface.h"
#include "Interaction/InteractionInterface.h"

UInventoryComponent::UInventoryComponent()
{	
	PrimaryComponentTick.bCanEverTick = false;
	Inventory = TArray<FSlot>();	
}

void UInventoryComponent::StoreItem(int32 SlotNumber, const bool bIsRightHand)
{
	AActor* StoredItem = bIsRightHand ? IHandInterface::Execute_GetRightHandItem(GetOwner()) : StoredItem = IHandInterface::Execute_GetLeftHandItem(GetOwner());
	if (!StoredItem) return;
	if (!IInteractionInterface::Execute_IsStorable(StoredItem)) return;
	
	const bool bIsFlashlight = IInteractionInterface::Execute_IsFlashLight(StoredItem); 
	SlotNumber = bIsFlashlight ? 0 : SlotNumber;
	if (!IsSlotAvailable(SlotNumber, bIsFlashlight)) return;

	const FSlot NewSlot = FSlot(SlotNumber, StoredItem->GetClass(), IInteractionInterface::Execute_GetIcon(StoredItem));
	Inventory.Add(NewSlot);
	OnItemStored.Broadcast(NewSlot, bIsRightHand);
}

void UInventoryComponent::WithdrawItem(const int32 SlotNumber, const bool bIsRightHand)
{
	FSlot FoundSlot = GetSlot(SlotNumber);
	if (FoundSlot.SlotNumber < 0) return;

	AActor* WithdrewItem = NewObject<AActor>(this, FoundSlot.SlotItemClass);
	if (bIsRightHand)
	{
		IHandInterface::Execute_SetRightHandItem(GetOwner(), WithdrewItem);
	}
	else
	{
		IHandInterface::Execute_SetLeftHandItem(GetOwner(), WithdrewItem);
	}
	Inventory.RemoveSingle(FoundSlot);
	OnItemWithdrew.Broadcast(SlotNumber);
}

bool UInventoryComponent::IsSlotAvailable(const int32 SlotNumber, const bool bIsFlashlight)
{
	if (GetSlot(SlotNumber).SlotNumber >= 0) return false;
	const bool IsSlotZero = SlotNumber == 0;	
	return !(bIsFlashlight ^ IsSlotZero);
}

FSlot UInventoryComponent::GetSlot(const int32 SlotNumber)
{
	FSlot Slot = FSlot();
	if (!Inventory.IsEmpty())
	{
		FSlot* FoundSlot = Inventory.FindByPredicate([SlotNumber](const FSlot& Slot)
		{
			return Slot.SlotNumber == SlotNumber;
		});
		if (FoundSlot)
		{
			Slot = *FoundSlot;
		}
	}
	return Slot;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();	
}
