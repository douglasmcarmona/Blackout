#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UsageInterface.generated.h"

UINTERFACE(MinimalAPI)
class UUsageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Objects calling this interface wish to put implementing objects into use. Each implementing object implements these
 * functions considering their conditions and resulting effects
 */
class BLACKOUT_API IUsageInterface
{
	GENERATED_BODY()
	
public:
	/**
	 * Called on an implementing object to check if it can actually be used at the given time 
	 * @param bIsRightHand True if it's the item in the right hand that should be used. False if it's the one in the left hand
	 * instead
	 * @return True if the object can perform its Use operation. False otherwise 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usage")
	bool CanBeUsed(bool bIsRightHand);

	/**
	 * Each implementing object must implement this function to cause their usage resulting effects
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usage")
	void Use();
};
