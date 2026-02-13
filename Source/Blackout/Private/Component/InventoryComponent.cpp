// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"

#include "Interaction/FlashlightInterface.h"
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
	
	const bool bIsFlashlight = StoredItem->Implements<UFlashlightInterface>(); 
	SlotNumber = bIsFlashlight ? 0 : SlotNumber;
	if (!IsSlotAvailable(SlotNumber, bIsFlashlight)) return;

	if (bIsFlashlight)
	{
		BatteryPercentage = IFlashlightInterface::Execute_GetBatteryPercentage(StoredItem);
		bIsFlashlightOn = IFlashlightInterface::Execute_IsFlashlightOn(StoredItem);
		const float DischargeRate = IFlashlightInterface::Execute_GetDischargeRate(StoredItem);
		OnFlashlightStored.Broadcast(BatteryPercentage, bIsFlashlightOn, DischargeRate);
	}
	
	const FSlot NewSlot = FSlot(SlotNumber, StoredItem->GetClass(), IInteractionInterface::Execute_GetIcon(StoredItem));
	Inventory.Add(NewSlot);
	OnItemStored.Broadcast(NewSlot, bIsRightHand);
}

void UInventoryComponent::WithdrawItem(const int32 SlotNumber, const bool bIsRightHand)
{
	const FSlot& FoundSlot = GetSlot(SlotNumber);
	if (FoundSlot.SlotNumber < 0) return;	
	if (IHandInterface::Execute_IsHandHoldingItem(GetOwner(), bIsRightHand)) return;
	
	FTransform Transform;
	Transform.SetLocation(IHandInterface::Execute_GetHandLocation(GetOwner(), bIsRightHand));
	
	AActor* WithdrewItem = GetWorld()->SpawnActorDeferred<AActor>(
		FoundSlot.SlotItemClass,
		Transform,
		GetOwner());
	
	if (!WithdrewItem) return;
	
	if (bIsRightHand)
	{
		IHandInterface::Execute_SetRightHandItem(GetOwner(), WithdrewItem);
	}
	else
	{
		IHandInterface::Execute_SetLeftHandItem(GetOwner(), WithdrewItem);
	}
	WithdrewItem->FinishSpawning(Transform);
	Inventory.RemoveSingle(FoundSlot);
	OnItemWithdrew.Broadcast(SlotNumber);
	
	if (WithdrewItem->Implements<UFlashlightInterface>())
	{
		IFlashlightInterface::Execute_Initialize(WithdrewItem);
		IFlashlightInterface::Execute_SetBatteryPercentage(WithdrewItem, BatteryPercentage);
		if (bIsFlashlightOn)
		{
			IFlashlightInterface::Execute_TurnOn(WithdrewItem);
		}
		else
		{
			IFlashlightInterface::Execute_TurnOff(WithdrewItem);
		}
	}
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
