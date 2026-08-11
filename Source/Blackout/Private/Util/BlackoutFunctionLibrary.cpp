#include "Util/BlackoutFunctionLibrary.h"

#include "JsonObjectConverter.h"
#include "Game/BlackoutGameInstance.h"
#include "Game/BlackoutGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/BlackoutHUD.h"
#include "Util/Compliance.h"
#include "Util/Json.h"

UPaperNoteInfo* UBlackoutFunctionLibrary::GetPaperNoteInfo(const UObject* WorldContextObject)
{
	if (ABlackoutGameModeBase* GameMode = Cast<ABlackoutGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return GameMode->PaperNoteInformation;
	}
	return nullptr;
}

UInventoryItemInfo* UBlackoutFunctionLibrary::GetInventoryItemInfo(const UObject* WorldContextObject)
{
	if (ABlackoutGameModeBase* GameMode = Cast<ABlackoutGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return GameMode->InventoryItemInformation;
	}
	return nullptr;
}

void UBlackoutFunctionLibrary::TogglePauseButton(const UObject* WorldContextObject, const bool bVisible)
{
	if (!WorldContextObject) return;
	
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PlayerController) return;
	
	if (ABlackoutHUD* HUD = Cast<ABlackoutHUD>(PlayerController->GetHUD()))
	{
		HUD->TogglePauseButtonWidget(bVisible);
	}
}

bool UBlackoutFunctionLibrary::IsMusicEnabled(const UObject* WorldContextObject)
{
	if (const UBlackoutGameInstance* GameInstance = Cast<UBlackoutGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		return GameInstance->IsMusicEnabled();
	}
	
	return true;
}

bool UBlackoutFunctionLibrary::IsSFXEnabled(const UObject* WorldContextObject)
{
	if (const UBlackoutGameInstance* GameInstance = Cast<UBlackoutGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		return GameInstance->IsSFXEnabled();
	}
	
	return true;
}

void UBlackoutFunctionLibrary::ToggleMusic(const UObject* WorldContextObject)
{
	if (UBlackoutGameInstance* GameInstance = Cast<UBlackoutGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		GameInstance->ToggleMusic();
	}
}

void UBlackoutFunctionLibrary::ToggleSFX(const UObject* WorldContextObject)
{
	if (UBlackoutGameInstance* GameInstance = Cast<UBlackoutGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		GameInstance->ToggleSFX();
	}
}

FString& UBlackoutFunctionLibrary::GetLicenseUrl(const ELicenseID LicenseID)
{
	return LicenseUrls.FindChecked(LicenseID);
}

void UBlackoutFunctionLibrary::SaveAssetComplianceMetadata(FAssetComplianceMetadata& Metadata)
{
	Metadata.LicenseUrl = GetLicenseUrl(Metadata.LicenseID);
	const FString Path = FString::Printf(TEXT("%sProduction"), *FPaths::ProjectDir());
	FAssetComplianceDatabase DatabaseStruct;
	LoadAssetComplianceMetadata(DatabaseStruct);
	DatabaseStruct.Database.Add(Metadata);
	UJson::SaveUStructAsJson<FAssetComplianceDatabase>(DatabaseStruct, Path, ASSET_COMPLIANCE_FILE);	
}

void UBlackoutFunctionLibrary::LoadAssetComplianceMetadata(FAssetComplianceDatabase& DatabaseStruct)
{
	const FString Path = FString::Printf(TEXT("%sProduction"), *FPaths::ProjectDir());	
	UJson::LoadJsonAsStruct<FAssetComplianceDatabase>(DatabaseStruct, Path, ASSET_COMPLIANCE_FILE);	
}

FAssetComplianceMetadata UBlackoutFunctionLibrary::FindMetdataByAssetID(FAssetComplianceDatabase DatabaseStruct,
	const FString& AssetID)
{
	for (const FAssetComplianceMetadata& Metadata : DatabaseStruct.Database)
	{
		if (Metadata.AssetID == AssetID) return Metadata;
	}
	return FAssetComplianceMetadata();
}
