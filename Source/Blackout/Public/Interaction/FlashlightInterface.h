#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FlashlightInterface.generated.h"

UINTERFACE()
class UFlashlightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Provides calling objects a way to call functions in the Flashlight actor, which will handle all events properly 
 * Main Implementer: BP_Flashlight
 */
class BLACKOUT_API IFlashlightInterface
{
	GENERATED_BODY()
	
public:
	/**
	 * Changes the BatteryPercentage property in the implementing class. Allows the battery actor to charge the flashlight's
	 * battery
	 * @param Percent The new battery percentage value 
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta=(ForceAsFunction), Category="Flashlight")
	void SetBatteryPercentage(const float Percent);
};
