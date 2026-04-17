#include "Game/BlackoutGameMode.h"
#include "Interaction/HandInterface.h"
#include "Kismet/GameplayStatics.h"

void ABlackoutGameMode::TravelToMap(const FString& MapName)
{
	if (IHandInterface* Player = Cast<IHandInterface>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		Player->SaveInventory();
	}
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Maps.FindChecked(MapName));
}
