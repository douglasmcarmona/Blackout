#include "Game/BlackoutGameInstance.h"

void UBlackoutGameInstance::SaveInventorySlotData(const FGuid& PersistentGuid, const int32 SlotNumber, const FString& ItemName,
	const TMap<FString, int32>& IntegerMap, const TMap<FString, float>& FloatMap, const TMap<FString, bool>& BoolMap)
{
	FInventorySlotData InventorySlotData;
	InventorySlotData.PersistentGuid = PersistentGuid;
	InventorySlotData.ItemName = ItemName;
	InventorySlotData.SlotNumber = SlotNumber;
	
	FInventorySlotMapData InventorySlotMapData;
	InventorySlotMapData.IntegerMap.Append(IntegerMap);
	InventorySlotMapData.FloatMap.Append(FloatMap);
	InventorySlotMapData.BoolMap.Append(BoolMap);
	
	InventorySlotData.InventorySlotMapData = InventorySlotMapData;
	InventoryData.Add(InventorySlotData);
}

bool UBlackoutGameInstance::LoadInventorySlotData(const int32 SlotNumber, FGuid& PersistentGuid,  FString& ItemName, TMap<FString, int32>& IntegerMap,
	TMap<FString, float>& FloatMap, TMap<FString, bool>& BoolMap)
{
	const FInventorySlotData* InventorySlotData = InventoryData.FindByPredicate([SlotNumber](const FInventorySlotData& InInventorySlotData)
	{
		return InInventorySlotData.SlotNumber == SlotNumber;
	});
	
	if (InventorySlotData)
	{
		PersistentGuid = InventorySlotData->PersistentGuid;
		ItemName = InventorySlotData->ItemName;
		IntegerMap.Append(InventorySlotData->InventorySlotMapData.IntegerMap);
		FloatMap.Append(InventorySlotData->InventorySlotMapData.FloatMap);
		BoolMap.Append(InventorySlotData->InventorySlotMapData.BoolMap);
		return true;
	}
	return false;
}

bool UBlackoutGameInstance::AddToPlacedActors(const FString& MapName, const FGuid& ActorGuid, const FPlacedActorData& PlacedActor)
{
	FLevelData* LevelData = LevelTransitionData.Find(MapName);
	if (!LevelData) return false;
	
	LevelData->PlacedActorsData.Add(ActorGuid, PlacedActor);
	return true;
}

bool UBlackoutGameInstance::AddToSpawnedActors(const FString& MapName,
	const FSpawnedActorData& SpawnedActor)
{
	FLevelData* LevelData = LevelTransitionData.Find(MapName);
	if (!LevelData) return false;

	LevelData->SpawnedActorsData.AddUnique(SpawnedActor);
	return true;
}
