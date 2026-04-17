#include "Game/BlackoutGameInstance.h"

void UBlackoutGameInstance::SaveInventorySlotData(const int32 SlotNumber, const FString& ItemName,
	const TMap<FString, int32>& IntegerMap, const TMap<FString, float>& FloatMap, const TMap<FString, bool>& BoolMap)
{
	FInventorySlotData InventorySlotData;
	InventorySlotData.ItemName = ItemName;
	InventorySlotData.SlotNumber = SlotNumber;
	
	FInventorySlotMapData InventorySlotMapData;
	InventorySlotMapData.IntegerMap.Append(IntegerMap);
	InventorySlotMapData.FloatMap.Append(FloatMap);
	InventorySlotMapData.BoolMap.Append(BoolMap);
	
	InventorySlotData.InventorySlotMapData = InventorySlotMapData;
	InventoryData.Add(InventorySlotData);
}

bool UBlackoutGameInstance::LoadInventorySlotData(const int32 SlotNumber, FString& ItemName, TMap<FString, int32>& IntegerMap,
	TMap<FString, float>& FloatMap, TMap<FString, bool>& BoolMap)
{
	const FInventorySlotData* InventorySlotData = InventoryData.FindByPredicate([SlotNumber](const FInventorySlotData& InInventorySlotData)
	{
		return InInventorySlotData.SlotNumber == SlotNumber;
	});
	
	if (InventorySlotData)
	{
		ItemName = InventorySlotData->ItemName;
		IntegerMap.Append(InventorySlotData->InventorySlotMapData.IntegerMap);
		FloatMap.Append(InventorySlotData->InventorySlotMapData.FloatMap);
		BoolMap.Append(InventorySlotData->InventorySlotMapData.BoolMap);
		return true;
	}
	return false;
}

