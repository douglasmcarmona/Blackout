#include "Game/BlackoutGameMode.h"

#include "EngineUtils.h"
#include "Game/BlackoutGameInstance.h"
#include "Interaction/HandInterface.h"
#include "Interaction/InteractionInterface.h"
#include "Kismet/GameplayStatics.h"

void ABlackoutGameMode::BeginPlay()
{
	Super::BeginPlay();
	UBlackoutGameInstance* BlackoutGameInstance = GetGameInstance<UBlackoutGameInstance>();	
	
	if (!BlackoutGameInstance->LevelTransitionData.Contains(GetWorld()->GetMapName()))
	{
		BlackoutGameInstance->LevelTransitionData.Add(GetWorld()->GetMapName(), FLevelData());
	}
}

void ABlackoutGameMode::SavePlacedActors() const
{
	UBlackoutGameInstance* BlackoutGameInstance = GetGameInstance<UBlackoutGameInstance>();
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	
	if (FLevelData* LevelData = BlackoutGameInstance->LevelTransitionData.Find(GetWorld()->GetMapName()))
	{
		for (FActorIterator It(GetWorld()); It; ++It)
		{			
			AActor* Actor = *It;			
			if (!IsValid(Actor) || !Actor->Implements<UInteractionInterface>()) continue;
			
			const bool bIsInPlayerHand = 
				Actor == IHandInterface::Execute_GetLeftHandItem(Player) || 
				Actor == IHandInterface::Execute_GetRightHandItem(Player);
			
			if (!IInteractionInterface::Execute_IsInOriginalState(Actor)) continue;
			
			const FGuid& PersistentGuid = IInteractionInterface::Execute_GetPersistentGuid(Actor);
			check(LevelData->PlacedActorsData.Find(PersistentGuid));
			
			FPlacedActorData ActorData;
			ActorData.ActorTransform = Actor->GetActorTransform();
			ActorData.bIsPendingKill = bIsInPlayerHand;
			
			FMemoryWriter MemoryWriter(ActorData.Bytes);
			MemoryWriter.GetArchiveState().ArIsSaveGame = true;
			Actor->Serialize(MemoryWriter);
			BlackoutGameInstance->AddToPlacedActors(GetWorld()->GetMapName(), PersistentGuid, ActorData);
		}
	}
}

void ABlackoutGameMode::SaveSpawnedActors() const
{
	UBlackoutGameInstance* BlackoutGameInstance = GetGameInstance<UBlackoutGameInstance>();
	APawn* Player =  UGameplayStatics::GetPlayerPawn(this, 0);
	
	if (BlackoutGameInstance->LevelTransitionData.Find(GetWorld()->GetMapName()))
	{
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			const bool bIsInPlayerHand = Actor == IHandInterface::Execute_GetLeftHandItem(Player) || 
				Actor == IHandInterface::Execute_GetRightHandItem(Player);
			
			if (!IsValid(Actor) || !Actor->Implements<UInteractionInterface>()) continue;		
			if (IInteractionInterface::Execute_IsInOriginalState(Actor) || bIsInPlayerHand) continue;
			
			FSpawnedActorData ActorData;
			ActorData.ActorGuid = IInteractionInterface::Execute_GetPersistentGuid(Actor);
			ActorData.ActorClass = Actor->GetClass();
			ActorData.ActorTransform = Actor->GetActorTransform();			
			
			FMemoryWriter MemoryWriter(ActorData.Bytes);
			MemoryWriter.GetArchiveState().ArIsSaveGame = true;
			Actor->Serialize(MemoryWriter);
			
			BlackoutGameInstance->AddToSpawnedActors(GetWorld()->GetMapName(), ActorData);
		}
	}
}

void ABlackoutGameMode::TravelToMap(const FString& MapName)
{	
	if (IHandInterface* Player = Cast<IHandInterface>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		Player->SaveInventory();
	}
	
	SavePlacedActors();
	SaveSpawnedActors();
	IGameInstanceInterface::Execute_TravelToMap(UGameplayStatics::GetGameInstance(this), MapName);
}

void ABlackoutGameMode::LoadPlacedActors(FLevelData& LevelData) const
{	
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!(IsValid(Actor) && Actor->Implements<UInteractionInterface>())) continue;
			
		FPlacedActorData* ActorData = LevelData.PlacedActorsData.Find(IInteractionInterface::Execute_GetPersistentGuid(Actor));
		if (!ActorData) continue;
		
		if (ActorData->bIsPendingKill)
		{
			Actor->Destroy();
		}
		else
		{
			Actor->SetActorTransform(ActorData->ActorTransform);
			FMemoryReader MemoryReader(ActorData->Bytes);
			MemoryReader.GetArchiveState().ArIsSaveGame = true;
			Actor->Serialize(MemoryReader);
		}
	}
}

void ABlackoutGameMode::LoadSpawnedActors(FLevelData& LevelData) const
{
	if (LevelData.SpawnedActorsData.IsEmpty()) return;
	
	for (const FSpawnedActorData& ActorData : LevelData.SpawnedActorsData)
	{
		AActor* Actor = GetWorld()->SpawnActorDeferred<AActor>(ActorData.ActorClass, ActorData.ActorTransform);
		FMemoryReader Reader(ActorData.Bytes);
		Reader.GetArchiveState().ArIsSaveGame = true;
		Actor->Serialize(Reader);
		Actor->FinishSpawning(ActorData.ActorTransform);
	}
	LevelData.SpawnedActorsData.Empty();
}

void ABlackoutGameMode::StartPlay()
{
	Super::StartPlay();
	UBlackoutGameInstance* BlackoutGameInstance = GetGameInstance<UBlackoutGameInstance>();
	FLevelData* LevelData = BlackoutGameInstance->LevelTransitionData.Find(GetWorld()->GetMapName());
	if (!LevelData) return;
	
	if (LevelData->bIsFirstLoad)
	{
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (IsValid(Actor) && Actor->Implements<UInteractionInterface>())
			{
				FPlacedActorData PlacedActor = FPlacedActorData();				
				PlacedActor.ActorTransform = Actor->GetActorTransform();
				BlackoutGameInstance->AddToPlacedActors(GetWorld()->GetMapName(), IInteractionInterface::Execute_GetPersistentGuid(Actor), PlacedActor);
			}
		}
		LevelData->bIsFirstLoad = false;
	}
	else
	{
		LoadPlacedActors(*LevelData);
		LoadSpawnedActors(*LevelData);
	}
}

void ABlackoutGameMode::MarkActorAsPendingKill(const FString& MapName, const FGuid& PersistentGuid) const
{
	UBlackoutGameInstance* BlackoutGameInstance = GetGameInstance<UBlackoutGameInstance>();
	
	FLevelData* LevelData = BlackoutGameInstance->LevelTransitionData.Find(MapName);
	if (!LevelData) return;
	
	FPlacedActorData* ActorData = LevelData->PlacedActorsData.Find(PersistentGuid);
	if (!ActorData) return;
	
	ActorData->bIsPendingKill = true;
	
	BlackoutGameInstance->AddToPlacedActors(MapName, PersistentGuid, *ActorData);
}
