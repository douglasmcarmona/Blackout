// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "Component/InventoryComponent.h"

void UInventoryWidgetController::StoreItem(const int32 SlotNumber, const bool bIsRightHand)
{
	InventoryComponent->StoreItem(SlotNumber, bIsRightHand);
}

void UInventoryWidgetController::WithdrawItem(const int32 SlotNumber, const bool bIsRightHand)
{
	if (SlotNumber == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(BatteryDischargeTimerHandle);
		InventoryComponent->BatteryPercentage = FlashlightBatteryPercentage;
		InventoryComponent->bIsFlashlightOn = bIsFlashlightOn;
	}
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
	InventoryComponent->OnFlashlightStored.AddLambda([this](
		const float InBatteryPercentage, const bool InIsFlashlightOn, const float DischargeRate)
	{
		FlashlightBatteryPercentage = InBatteryPercentage;
		bIsFlashlightOn = InIsFlashlightOn;
		OnFlashlightBatteryChangedDelegate.Broadcast(FlashlightBatteryPercentage);
		if (bIsFlashlightOn)
		{
			GetWorld()->GetTimerManager().SetTimer(
				BatteryDischargeTimerHandle,
				[this]
				{
					FlashlightBatteryPercentage = FMath::Clamp(--FlashlightBatteryPercentage, 0.f, 100.f);	
					OnFlashlightBatteryChangedDelegate.Broadcast(FlashlightBatteryPercentage);
				},
				DischargeRate,
				true
				);
		}
	});
}

void UInventoryWidgetController::LoadInventory()
{
	for (int32 i=0; i<InventoryComponent->InventorySize; i++)
	{
		FSlot Slot = InventoryComponent->GetSlot(i);	
		OnInventoryOpenDelegate.Broadcast(i, Slot.SlotNumber >= 0, Slot, FlashlightBatteryPercentage);
	}
}

UWorld* UInventoryWidgetController::GetWorld() const
{
	return GetOuter()->GetWorld();
}
