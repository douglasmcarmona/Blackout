#include "Util/BlackoutFunctionLibrary.h"

#include "Game/BlackoutGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/BlackoutHUD.h"

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
