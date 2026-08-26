#include "Util/BlackoutFunctionLibrary.h"
#include "Game/BlackoutGameInstance.h"
#include "Game/BlackoutGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BlackoutPlayerController.h"
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
		HUD->TogglePauseButton(bVisible);
	}
}

void UBlackoutFunctionLibrary::ToggleGamePaused(const UObject* WorldContextObject, const bool bGamePaused)
{
	if (!WorldContextObject) return;
	
	ABlackoutPlayerController* PlayerController = Cast<ABlackoutPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0));
	if (!PlayerController) return;
	
	ABlackoutHUD* HUD = Cast<ABlackoutHUD>(PlayerController->GetHUD());
	if (!HUD) return;
	
	HUD->TogglePauseButton(!bGamePaused);
	HUD->TogglePauseMenu(bGamePaused);	
	
	if (bGamePaused)
	{
		PlayerController->ChangeMappingContext(EMappingContext::PauseMenu);
		PlayerController->SetInputMode(FInputModeGameAndUI());
		
	}
	else
	{
		PlayerController->ChangeMappingContext(EMappingContext::Default);
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
	
	PlayerController->SetShowMouseCursor(bGamePaused);
	UGameplayStatics::SetGamePaused(WorldContextObject, bGamePaused);
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
	FAssetComplianceDatabase DatabaseStruct;
	LoadAssetComplianceMetadata(DatabaseStruct);
	DatabaseStruct.Database.Add(Metadata);
	UJson::SaveUStructAsJson<FAssetComplianceDatabase>(DatabaseStruct, TEXT(ASSET_COMPLIANCE_FILE_DIR), TEXT(ASSET_COMPLIANCE_FILE));
}

void UBlackoutFunctionLibrary::LoadAssetComplianceMetadata(FAssetComplianceDatabase& DatabaseStruct)
{
	UJson::LoadJsonAsStruct<FAssetComplianceDatabase>(DatabaseStruct, TEXT(ASSET_COMPLIANCE_FILE_DIR), TEXT(ASSET_COMPLIANCE_FILE));	
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

FString UBlackoutFunctionLibrary::GetLicenseDescriptionText(const ELicenseID LicenseID)
{	
	switch (LicenseID)
	{
		case ELicenseID::Fab:
			return FString(TEXT("Fab Standard License"));		
		case ELicenseID::CC_0:
			return FString(TEXT("Creative Commons 0"));
		case ELicenseID::CC_BY_4:
			return FString(TEXT("Creative Commons by (Attribution)"));	
		case ELicenseID::UE_EULA:
			return FString(TEXT("Unreal Engine's End User License Agreement"));		
		default:
			return FString();
	}	
}
