#include "Game/BlackoutGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BlackoutPlayerController.h"
#include "Framework/Application/SlateApplication.h"
#include "UI/HUD/BlackoutHUD.h"

void UBlackoutGameInstance::Init()
{
	Super::Init();
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get()
			.OnApplicationActivationStateChanged()
			.AddUObject(this, &UBlackoutGameInstance::OnApplicationActivationChanged);		
	}	
}

void UBlackoutGameInstance::SaveInventorySlotData(const FGuid& PersistentGuid, const int32 SlotNumber, const FString& ItemName,
                                                  const TMap<FString, int32>& IntegerMap, const TMap<FString, float>& FloatMap, const TMap<FString, bool>& BoolMap)
{
	FInventorySlotData InventorySlotData;
	InventorySlotData.PersistentGuid = PersistentGuid;
	InventorySlotData.ItemName = ItemName;
	InventorySlotData.SlotNumber = SlotNumber;
	
	FInventorySlotMapData InventorySlotMapData;
	InventorySlotMapData.IntegerMap.Append(IntegerMap);
	InventorySlotMapData.FloatMap.Append(FloatMap);
	InventorySlotMapData.BoolMap.Append(BoolMap);
	
	InventorySlotData.InventorySlotMapData = InventorySlotMapData;
	InventoryData.Add(InventorySlotData);
}

bool UBlackoutGameInstance::LoadInventorySlotData(const int32 SlotNumber, FGuid& PersistentGuid,  FString& ItemName, TMap<FString, int32>& IntegerMap,
	TMap<FString, float>& FloatMap, TMap<FString, bool>& BoolMap)
{
	const FInventorySlotData* InventorySlotData = InventoryData.FindByPredicate([SlotNumber](const FInventorySlotData& InInventorySlotData)
	{
		return InInventorySlotData.SlotNumber == SlotNumber;
	});
	
	if (InventorySlotData)
	{
		PersistentGuid = InventorySlotData->PersistentGuid;
		ItemName = InventorySlotData->ItemName;
		IntegerMap.Append(InventorySlotData->InventorySlotMapData.IntegerMap);
		FloatMap.Append(InventorySlotData->InventorySlotMapData.FloatMap);
		BoolMap.Append(InventorySlotData->InventorySlotMapData.BoolMap);
		return true;
	}
	return false;
}

bool UBlackoutGameInstance::AddToPlacedActors(const FString& MapName, const FGuid& ActorGuid, const FPlacedActorData& PlacedActor)
{
	FLevelData* LevelData = LevelTransitionData.Find(MapName);
	if (!LevelData) return false;
	
	LevelData->PlacedActorsData.Add(ActorGuid, PlacedActor);
	return true;
}

bool UBlackoutGameInstance::AddToSpawnedActors(const FString& MapName,
	const FSpawnedActorData& SpawnedActor)
{
	FLevelData* LevelData = LevelTransitionData.Find(MapName);
	if (!LevelData) return false;

	LevelData->SpawnedActorsData.AddUnique(SpawnedActor);
	return true;
}

void UBlackoutGameInstance::ToggleMusic()
{
	bIsMusicEnabled = !bIsMusicEnabled;
	OnMusicToggledDelegate.Broadcast(bIsMusicEnabled);
}

void UBlackoutGameInstance::ToggleSFX()
{
	bIsSFXEnabled = !bIsSFXEnabled;
	OnSFXToggledDelegate.Broadcast(bIsSFXEnabled);
}

void UBlackoutGameInstance::TravelToMap_Implementation(const FString& MapName)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Maps.FindChecked(MapName));
}

void UBlackoutGameInstance::ToggleGamePaused(const UObject* WorldContextObject, const bool bGamePaused) const
{	
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

void UBlackoutGameInstance::OnApplicationActivationChanged(bool bIsActive) const
{
	const UWorld* World = GetWorld();
	if (!World) return;
	
	if (!bIsActive && !UGameplayStatics::IsGamePaused(World))
	{
		ToggleGamePaused(World, true);
	}
	else if (bIsActive && UGameplayStatics::IsGamePaused(World))
	{		
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		GetFirstLocalPlayerController(World)->SetInputMode(InputMode);
	}
}

void UBlackoutGameInstance::Shutdown()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get()
			.OnApplicationActivationStateChanged()
			.RemoveAll(this);
	}
	Super::Shutdown();
}
