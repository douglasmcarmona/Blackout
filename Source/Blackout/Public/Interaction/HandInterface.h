// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HandInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHandInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLACKOUT_API IHandInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hand")
	AActor* GetRightHandItem() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hand")
	AActor* GetLeftHandItem() const;
};
