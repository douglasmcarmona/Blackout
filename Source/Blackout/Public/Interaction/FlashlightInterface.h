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
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	float GetBatteryPercentage() const;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta=(ForceAsFunction))
	void SetBatteryPercentage(const float Percent);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	bool IsFlashlightOn() const;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	float GetDischargeRate() const;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta=(ForceAsFunction))
	void Initialize();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta=(ForceAsFunction))
	void TurnOn();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta=(ForceAsFunction))
	void TurnOff();
};
