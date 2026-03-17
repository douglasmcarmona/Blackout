// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FlashlightInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UFlashlightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLACKOUT_API IFlashlightInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta=(ForceAsFunction), Category="Flashlight")
	void SetBatteryPercentage(const float Percent);
};
