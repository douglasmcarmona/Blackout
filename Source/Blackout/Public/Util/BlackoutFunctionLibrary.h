#pragma once

#include "CoreMinimal.h"
#include "Compliance.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlackoutFunctionLibrary.generated.h"

class UInventoryItemInfo;
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

	/**
	 * Retrieves the InventoryItemInfo data asset from its source (the GameMode)
	 * @param WorldContextObject An existing object in the world to provide context for this function 
	 * @return The InventoryItemInfo object
	 */
	UFUNCTION(BlueprintPure, Category="Blackout|Data", meta = (DefaultToSelf = "WorldContextObject"))
	static UInventoryItemInfo* GetInventoryItemInfo(const UObject* WorldContextObject);

	/**
	 * Controls visibility of the pause game widget 
	 * @param WorldContextObject An existing object in the world to provide context for this function
	 * @param bVisible True if the widget must be visible. False otherwise
	 */
	UFUNCTION(BlueprintCallable, Category="Blackout|Pause", meta = (DefaultToSelf = "WorldContextObject"))
	static void TogglePauseButton(const UObject* WorldContextObject, const bool bVisible);

	/**
	 * Checks the music general control status
	 * @param WorldContextObject An existing object in the world to provide context for this function
	 * @return True if music is currently enabled. False otherwise
	 */
	UFUNCTION(BlueprintPure, Category="Blackout|Audio", meta = (DefaultToSelf = "WorldContextObject"))
	static bool IsMusicEnabled(const UObject* WorldContextObject);
	
	/**
	 * Checks sound effects general control status
	 * @param WorldContextObject An existing object in the world to provide context for this function
	 * @return True if sound effects are currently enabled. False otherwise
	 */
	UFUNCTION(BlueprintPure, Category="Blackout|Audio", meta = (DefaultToSelf = "WorldContextObject"))
	static bool IsSFXEnabled(const UObject* WorldContextObject);

	/**
	 * Flips music general control value
	 * @param WorldContextObject An existing object in the world to provide context for this function
	 */
	UFUNCTION(BlueprintCallable, Category="Blackout|Audio", meta = (DefaultToSelf = "WorldContextObject"))
	static void ToggleMusic(const UObject* WorldContextObject);

	/**
	 * Flips sound effects general control value
	 * @param WorldContextObject An existing object in the world to provide context for this function	 
	 */
	UFUNCTION(BlueprintCallable, Category="Blackout|Audio", meta = (DefaultToSelf = "WorldContextObject"))
	static void ToggleSFX(const UObject* WorldContextObject);

	/**
	 * Handy function to return the URL of a license from its enum value
	 * @param LicenseID The value which identifies the license
	 * @return The URL of the license
	 */
	UFUNCTION(BlueprintPure, Category="Blackout|Compliance")
	static FString& GetLicenseUrl(const ELicenseID LicenseID);

	/**
	 * Writes a new compliance metadata data structure to our data file
	 * @param Metadata All data to be written in the file
	 */
	UFUNCTION(BlueprintCallable, Category="Blackout|Compliance")
	static void SaveAssetComplianceMetadata(UPARAM(ref) FAssetComplianceMetadata& Metadata);

	/**
	 * Reads our compliance data file into memory
	 * @param DatabaseStruct (Output) Filled up by the function with all parsed data from the source file
	 */
	UFUNCTION(BlueprintCallable, Category="Blackout|Compliance")
	static void LoadAssetComplianceMetadata(FAssetComplianceDatabase& DatabaseStruct);

	/**
	 * Gets a specific entry from the compliance database
	 * @param DatabaseStruct The compliance database
	 * @param AssetID The ID of the asset data to be searched for
	 * @return The data structure which represents the compliance data regarding the asset
	 */
	UFUNCTION(BlueprintPure, Category="Blackout|Compliance")
	static FAssetComplianceMetadata FindMetdataByAssetID(FAssetComplianceDatabase DatabaseStruct, const FString& AssetID);
};
