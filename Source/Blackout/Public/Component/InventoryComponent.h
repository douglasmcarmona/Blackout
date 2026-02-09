// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventorySlot;

USTRUCT(BlueprintType)
struct FSlot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 SlotNumber = -1;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<AActor> SlotItemClass = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTexture2D> SlotIcon = nullptr;

	bool operator==(const FSlot& Other) const
	{
		return SlotNumber == Other.SlotNumber;
	}	
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemStored, const FSlot&, const bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemWithdrew, const int32)

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class BLACKOUT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();
	void StoreItem(int32 SlotNumber, const bool bIsRightHand);
	void WithdrawItem(const int32 SlotNumber, const bool bIsRightHand);

	UFUNCTION(BlueprintCallable)
	bool IsSlotAvailable(const int32 SlotNumber, const bool bIsFlashlight);

	UFUNCTION(BlueprintCallable)
	FSlot GetSlot(const int32 SlotNumber);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 InventorySize = 5;
	
	FOnItemStored OnItemStored;
	FOnItemWithdrew OnItemWithdrew;

private:
	UPROPERTY(VisibleAnywhere)
	TArray<FSlot> Inventory;
};
