#include "Component/InventoryComponent.h"

#include "Data/InventoryItemInfo.h"
#include "Interaction/FlashlightInterface.h"
#include "Interaction/HandInterface.h"
#include "Interaction/InteractionInterface.h"
#include "Util/BlackoutFunctionLibrary.h"

UInventoryComponent::UInventoryComponent()
{	
	PrimaryComponentTick.bCanEverTick = false;
	Inventory = TArray<FSlot>();
}

void UInventoryComponent::StoreItem(int32 SlotNumber, const bool bIsRightHand)
{
	if (Inventory.Num() >= InventorySize) return;
		
	AActor* StoredItem = bIsRightHand ? 
		IHandInterface::Execute_GetRightHandItem(GetOwner()) :
		StoredItem = IHandInterface::Execute_GetLeftHandItem(GetOwner());
	
	if (!StoredItem) return;
	if (!IInteractionInterface::Execute_IsStorable(StoredItem)) return;
	
	const bool bIsFlashlight = StoredItem->Implements<UFlashlightInterface>();
	SlotNumber = bIsFlashlight ? 0 : SlotNumber;
	if (!IsSlotAvailable(SlotNumber, bIsFlashlight)) return;
	
	FSlot NewSlot = FSlot(
		SlotNumber,
		StoredItem->GetClass(),
		IInteractionInterface::Execute_GetIcon(StoredItem),
		FSlotData());
	
	IInteractionInterface::Execute_HandleStoredItemSlotData(StoredItem, NewSlot.SlotData);
	Inventory.Add(NewSlot);
	if (bIsFlashlight)
	{		
		OnFlashlightStored.Broadcast();
	}
	OnItemStored.Broadcast(NewSlot, bIsRightHand);
}

void UInventoryComponent::WithdrawItem(const int32 SlotNumber, const bool bIsRightHand)
{
	if (Inventory.IsEmpty()) return;
	
	const FSlot* FoundSlot = GetSlot(SlotNumber);
	if (!FoundSlot) return;
	if (IHandInterface::Execute_IsHandHoldingItem(GetOwner(), bIsRightHand)) return;
	
	FTransform Transform;
	Transform.SetLocation(IHandInterface::Execute_GetHandLocation(GetOwner(), bIsRightHand));
	
	AActor* WithdrawnItem = GetWorld()->SpawnActorDeferred<AActor>(
		FoundSlot->SlotItemClass,
		Transform,
		GetOwner());
	
	if (!WithdrawnItem) return;	
	
	if (bIsRightHand)
	{
		IHandInterface::Execute_SetRightHandItem(GetOwner(), WithdrawnItem);
	}
	else
	{
		IHandInterface::Execute_SetLeftHandItem(GetOwner(), WithdrawnItem);
	}
	
	WithdrawnItem->FinishSpawning(Transform);
	IInteractionInterface::Execute_HandleWithdrawnItemSlotData(WithdrawnItem, FoundSlot->SlotData);
	Inventory.RemoveSingle(*FoundSlot);
	OnItemWithdrawn.Broadcast(SlotNumber);
}

void UInventoryComponent::RestoreItem(const int32 SlotNumber, const FString& ItemName, const TMap<FString, int32>& IntegerMap,
	const TMap<FString, float>& FloatMap, const TMap<FString, bool>& BoolMap)
{
	UInventoryItemInfo* InventoryItemInfo = UBlackoutFunctionLibrary::GetInventoryItemInfo(GetOwner());
	if (!InventoryItemInfo) return;
	
	FInventoryItem* InventoryItem = InventoryItemInfo->GetInventoryItemByName(ItemName);
	if (!InventoryItem) return;
	
	FSlot Slot = FSlot();
	Slot.SlotNumber = SlotNumber;
	Slot.SlotItemClass = InventoryItem->ItemClass;
	Slot.SlotIcon = InventoryItem->ItemIcon;
	FSlotData SlotData = FSlotData();
	SlotData.IntegerValues.Append(IntegerMap);
	SlotData.FloatValues.Append(FloatMap);
	SlotData.BoolValues.Append(BoolMap);
	Slot.SlotData = SlotData;
	Inventory.Add(Slot);
}

bool UInventoryComponent::IsSlotAvailable(const int32 SlotNumber, const bool bIsFlashlight)
{
	if (GetSlot(SlotNumber)) return false;
	const bool IsSlotZero = SlotNumber == 0;	
	return !(bIsFlashlight ^ IsSlotZero);
}

FSlot* UInventoryComponent::GetSlot(const int32 SlotNumber)
{
	return Inventory.FindByPredicate([SlotNumber](const FSlot& Slot)
	{
		return Slot.SlotNumber == SlotNumber;
	});
}
