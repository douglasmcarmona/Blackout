// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventorySlot.generated.h"

class AInteractableActor;
/**
 * 
 */
UCLASS(BlueprintType)
class BLACKOUT_API UInventorySlot : public UObject
{
	GENERATED_BODY()

public:
	UInventorySlot();
	
	static UInventorySlot EmptySlot;
	
	UFUNCTION(BlueprintPure)
	int32 GetSlotNumber() const;
	void SetSlotNumber(const int32 InSlotNumber);
	
	UFUNCTION(BlueprintPure)
	TSubclassOf<AInteractableActor> GetSlotItemClass() const;	
	void SetSlotItemClass(UClass* InSlotItemClass);

	UFUNCTION(BlueprintPure)
	UTexture2D* GetSlotIcon() const;
	void SetSlotIcon(UTexture2D* InSlotIcon);

private:	
	int32 SlotNumber;
	TSubclassOf<AInteractableActor> SlotItemClass;

	UPROPERTY()
	TObjectPtr<UTexture2D> SlotIcon;	
};
