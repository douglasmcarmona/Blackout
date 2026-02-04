// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventorySlot;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemStored, const UInventorySlot*, const bool);

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class BLACKOUT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();
	void StoreItem(int32 SlotNumber, const bool bIsRightHand);

	UFUNCTION(BlueprintCallable)
	bool IsSlotAvailable(const int32 SlotNumber, const bool bIsFlashlight);

	UFUNCTION(BlueprintCallable)
	UInventorySlot* GetSlot(const int32 SlotNumber);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 InventorySize = 5;
	
	FOnItemStored OnItemStored;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TArray<UInventorySlot*> Inventory;
};
