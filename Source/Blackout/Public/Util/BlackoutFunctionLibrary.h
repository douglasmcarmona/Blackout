#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlackoutFunctionLibrary.generated.h"

class UPaperNoteInfo;

/**
 * Our custom blueprint function library that can be called from anywhere in the project to perform arbitrary functionalities
 */
UCLASS()
class BLACKOUT_API UBlackoutFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	 * Retrieves the PaperNoteInfo data asset from its source (the GameMode)
	 * @param WorldContextObject An existing object in the world to provide context for this function
	 * @return The PaperNoteInfo object
	 */
	UFUNCTION(BlueprintPure, Category="Blackout|Data", meta = (DefaultToSelf = "WorldContextObject"))
	static UPaperNoteInfo* GetPaperNoteInfo(const UObject* WorldContextObject);	
};
