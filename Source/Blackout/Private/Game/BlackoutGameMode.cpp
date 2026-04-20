#include "Game/BlackoutGameMode.h"

#include "Game/BlackoutGameInstance.h"
#include "Interaction/HandInterface.h"
#include "Kismet/GameplayStatics.h"

void ABlackoutGameMode::BeginPlay()
{
	Super::BeginPlay();
	UBlackoutGameInstance* BlackoutGameInstance = GetGameInstance<UBlackoutGameInstance>();
	if (!BlackoutGameInstance->LevelStoredItems.Contains(GetWorld()->GetMapName()))
	{
		BlackoutGameInstance->LevelStoredItems.Add(GetWorld()->GetMapName(), TSet<FGuid>());		
	}
}

void ABlackoutGameMode::TravelToMap(const FString& MapName)
{
	if (IHandInterface* Player = Cast<IHandInterface>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		Player->SaveInventory();
	}
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Maps.FindChecked(MapName));
}
