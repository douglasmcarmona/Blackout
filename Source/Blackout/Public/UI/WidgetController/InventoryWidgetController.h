// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidgetController.generated.h"

class UInventorySlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryOpen, int32, SlotNumber, const FSlot&, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemStoredSignature, int32, StoredItemSlotNumber, UTexture2D*,
                                             StoredItemIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemWithdrewSignature, int32, SlotNumber);

class UInventoryComponent;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BLACKOUT_API UInventoryWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void HandleItem(const int32 SlotNumber, const bool bIsRightHand, const bool bIsWithdrawing);

	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetInventoryComponent() const;

	UFUNCTION(BlueprintCallable)
	void SetInventoryComponent(UInventoryComponent* InInventoryComponent);

	UFUNCTION(BlueprintCallable)
	void LoadInventory();

	UPROPERTY(BlueprintAssignable)
	FOnItemStoredSignature OnItemStoredDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnItemWithdrewSignature OnItemWithdrewDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryOpen OnInventoryOpenDelegate;

private:
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
