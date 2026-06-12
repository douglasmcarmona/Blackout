#include "Util/BlackoutFunctionLibrary.h"

#include "Game/BlackoutGameModeBase.h"
#include "Kismet/GameplayStatics.h"

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
