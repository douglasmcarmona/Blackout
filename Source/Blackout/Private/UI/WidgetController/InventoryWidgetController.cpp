// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "Component/InventoryComponent.h"

#define BATTERY_PERCENTAGE_MAP_KEY "BatteryPercentage"
#define IS_FLASHLIGHT_ON_MAP_KEY "IsFlashlightOn"
#define DISCHARGE_RATE_MAP_KEY "DischargeRate"

void UInventoryWidgetController::StoreItem(const int32 SlotNumber, const bool bIsRightHand)
{
	InventoryComponent->StoreItem(SlotNumber, bIsRightHand);
}

void UInventoryWidgetController::WithdrawItem(const int32 SlotNumber, const bool bIsRightHand)
{
	if (SlotNumber == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(BatteryDischargeTimerHandle);		
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
	InventoryComponent->OnItemWithdrawn.AddLambda([this](const int32 SlotNumber)
	{
		OnItemWithdrewDelegate.Broadcast(SlotNumber);
	});
	InventoryComponent->OnFlashlightStored.AddUObject(this, &UInventoryWidgetController::DischargeFlashlightBattery);
}

void UInventoryWidgetController::LoadInventory() const
{
	const FSlot* FlashlightSlot = InventoryComponent->GetSlot(0);	
	const float BatteryPercentage = FlashlightSlot ?
		*FlashlightSlot->SlotData.FloatValues.Find(FString(BATTERY_PERCENTAGE_MAP_KEY)) : -1;
	OnInventoryOpenDelegate.Broadcast(0, FlashlightSlot != nullptr, FlashlightSlot ? *FlashlightSlot : FSlot(), BatteryPercentage);
	
	for (int32 i=1; i<InventoryComponent->InventorySize; i++)
	{
		const FSlot* Slot = InventoryComponent->GetSlot(i);
		OnInventoryOpenDelegate.Broadcast(i, Slot != nullptr, Slot ? *Slot : FSlot(), BatteryPercentage);
	}
}

UWorld* UInventoryWidgetController::GetWorld() const
{
	return GetOuter()->GetWorld();
}

void UInventoryWidgetController::DischargeFlashlightBattery()
{
	FSlot* FlashlightSlot = InventoryComponent->GetSlot(0);
	float BatteryPercentage = *FlashlightSlot->SlotData.FloatValues.Find(FString(BATTERY_PERCENTAGE_MAP_KEY));
	const bool bIsFlashlightOn = *FlashlightSlot->SlotData.BoolValues.Find(FString(IS_FLASHLIGHT_ON_MAP_KEY));
	OnFlashlightBatteryChangedDelegate.Broadcast(BatteryPercentage);
	if (bIsFlashlightOn)
	{
		const float BatteryDischargeRate = *FlashlightSlot->SlotData.FloatValues.Find(FString(DISCHARGE_RATE_MAP_KEY));
		GetWorld()->GetTimerManager().SetTimer(
			BatteryDischargeTimerHandle,
			[this, BatteryPercentage]() mutable
			{
				BatteryPercentage = FMath::Clamp(--BatteryPercentage, 0.f, 100.f);
				InventoryComponent->GetSlot(0)->SlotData.FloatValues.Add(FString(BATTERY_PERCENTAGE_MAP_KEY), BatteryPercentage);
				OnFlashlightBatteryChangedDelegate.Broadcast(BatteryPercentage);
			},
			BatteryDischargeRate,
			true
			);
	}
}
