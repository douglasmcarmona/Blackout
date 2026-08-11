#pragma once

#include "CoreMinimal.h"
#include "Compliance.generated.h"

/**
 * Header that defines all structures used to build compliance related data
 */

// Destination file of compliance asset metadata
#define ASSET_COMPLIANCE_FILE FString("AssetComplianceDatabase.json")


// Enumeration of all possible licenses a given asset used in the game can be licensed under 
UENUM(BlueprintType)
enum class ELicenseID : uint8
{
	None UMETA(DisplayName = "None"),
	Fab UMETA(DisplayName = "Fab Standard License"),
	CC_0 UMETA(DisplayName = "Creative Commons 0"),
	CC_BY_4 UMETA(DisplayName = "Creative Commons by (Attribution)"),
	UE_EULA UMETA(DisplayName = "Unreal Engine End User License Agreement (EULA)")
};

// Handy data structure to retrieve the URL's of all licenses
static inline TMap<ELicenseID, FString> LicenseUrls
{
	{ELicenseID::None, FString("")},
	{ELicenseID::Fab, FString("https://www.fab.com/eula")},
	{ELicenseID::CC_0, FString("https://creativecommons.org/publicdomain/zero/1.0/legalcode.en")},
	{ELicenseID::CC_BY_4, FString("https://creativecommons.org/licenses/by/4.0/legalcode.en")},
	{ELicenseID::UE_EULA, FString("https://www.unrealengine.com/eula/unreal")}
};

/**
 * Data structure that gathers all compliance related data of an external asset used in the game
 */
USTRUCT(BlueprintType)
struct FAssetComplianceMetadata
{
	GENERATED_BODY()
	
	// Property that identifies an asset
	UPROPERTY(BlueprintReadWrite)
	FString AssetID = "TP-0000";
	
	// The name of the asset
	UPROPERTY(BlueprintReadWrite)
	FString AssetName = "Invalid asset";
	
	// The original creator of the asset
	UPROPERTY(BlueprintReadWrite)
	FString Creator = "Invalid creator";
	
	// Identifies which license the asset is licensed under
	UPROPERTY(BlueprintReadWrite)
	ELicenseID LicenseID = ELicenseID::None;
	
	// Link to the location where the full license text can be found
	UPROPERTY(BlueprintReadOnly)
	FString LicenseUrl;
	
	// The platform from where the asset was imported
	UPROPERTY(BlueprintReadWrite)
	FString AssetSource = "Invalid source";
	
	// Lick to the location where the asset was found
	UPROPERTY(BlueprintReadWrite)
	FString AssetUrl = "Invalid url";
	
	// Indicates if the license requires that the asset must be attributed to its original creator    
	UPROPERTY(BlueprintReadWrite)
	bool AttributionRequired = true;
	
	// The explicit statement that attributes the asset to its original creator
	UPROPERTY(BlueprintReadWrite)
	FString AttributionText = "Invalid attribution";
	
	// Indicates if the asset differs from its original state in some way 
	UPROPERTY(BlueprintReadWrite)
	bool Modified = false;
	
	// Short description of modifications performed on the asset 
	UPROPERTY(BlueprintReadWrite)
	FString ModificationText = "Invalid notification";
};

/*
 * Holds all asset compliance metadata entries
 */
USTRUCT(BlueprintType)
struct FAssetComplianceDatabase
{	
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FAssetComplianceMetadata> Database = TArray<FAssetComplianceMetadata>();	
};
