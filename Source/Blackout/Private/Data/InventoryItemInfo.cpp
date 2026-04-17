#include "Data/InventoryItemInfo.h"

FInventoryItem* UInventoryItemInfo::GetInventoryItemByName(const FString& InventoryItemName)
{
	return InventoryItems.FindByPredicate([InventoryItemName](const FInventoryItem& InventoryItem)
	{
		return InventoryItem.ItemName == InventoryItemName;
	});
}

FInventoryItem* UInventoryItemInfo::GetInventoryItemByClass(UClass* InventoryItemClass)
{
	return InventoryItems.FindByPredicate([InventoryItemClass](const FInventoryItem& InventoryItem)
	{
		return InventoryItem.ItemClass == InventoryItemClass;
	});
}

#if WITH_EDITOR
void UInventoryItemInfo::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (InventoryItems.Num() <= 1) return;
	
	TSet<FString> ExistingItemNames;
	TSet<UClass*> ExistingItemClasses;
	
	for (int32 i=0; i<InventoryItems.Num(); i++)
	{
		if (ExistingItemNames.Contains(InventoryItems[i].ItemName.ToLower()) || ExistingItemClasses.Contains(InventoryItems[i].ItemClass))
		{
			UE_LOG(LogTemp, Warning, TEXT("There's already an item with the given name or class"));
			InventoryItems.RemoveAt(i);
		}
		else
		{
			ExistingItemNames.Add(InventoryItems[i].ItemName);
			ExistingItemClasses.Add(InventoryItems[i].ItemClass);
		}
	}
}
#endif
