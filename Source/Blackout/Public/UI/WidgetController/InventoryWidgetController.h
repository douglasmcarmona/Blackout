// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemStoredSignature, int32, StoredItemSlotNumber, UTexture2D*, StoredItemIcon);

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
	void StoreItem(const int32 SlotNumber, const bool bIsRightHand);

	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetInventoryComponent() const;

	UFUNCTION(BlueprintCallable)
	void SetInventoryComponent(UInventoryComponent* InInventoryComponent);

	UPROPERTY(BlueprintAssignable)
	FOnItemStoredSignature OnItemStoredDelegate;

private:
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
